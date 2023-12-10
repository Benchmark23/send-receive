#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include "utils.h"
#include "receiver.h"
#include "sender.h"
#include "tscns.h"

void thread_function(std::vector<entry> send_entries,
                     std::vector<long long> intvals, std::string self_ip,
                     std::string target_ip, int target_port, std::string logfile, double ghz)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);

    TCPSender t_sender;
    t_sender.ip = self_ip;
    t_sender.init_log(send_entries);

    UDPSender u_sender;

    if (t_sender.connect__(target_ip, target_port) != 0)
    {
        return;
    }

    std::vector<std::string> sent_buffer;

    TSCNS tscns;
    tscns.init();

    int i = 0;
    int upper = send_entries.size();
    while (i < upper)
    {
        auto now_cycle = tscns.rdtsc();
        if (tscns.tsc2ns(now_cycle) >= send_entries[i].timestamp)
        {
            auto send_res = t_sender.send__(send_entries[i].id, send_entries[i].size);
            if (send_res < 0)
            {
                std::cerr << "send error" << std::endl;
                break;
            }
            sent_buffer.push_back(send_entries[i].id);
            i++;
        }
        // assuming 500 cycle is enough for time conversion
        else if (tscns.tsc2ns(now_cycle + 500) < send_entries[i].timestamp && !sent_buffer.empty())
        {
            auto id = sent_buffer.back();
            t_sender.update_SL_time(id, tscns.tsc2ns(t_sender.SL_log[id].timestamp));
            t_sender.update_SR_time(id, tscns.tsc2ns(t_sender.SR_log[id].timestamp));
            sent_buffer.pop_back();
        }
        else if (sent_buffer.empty() && tscns.tsc2ns(now_cycle + 10000000) < send_entries[i].timestamp && !sent_buffer.empty())
        {
            tscns.calibrate();
        }
    }

    t_sender.disconnect__();

    while (!sent_buffer.empty())
    {
        auto id = sent_buffer.back();
        t_sender.update_SL_time(id, tscns.tsc2ns(t_sender.SL_log[id].timestamp));
        t_sender.update_SR_time(id, tscns.tsc2ns(t_sender.SR_log[id].timestamp));
        sent_buffer.pop_back();
    }

    flush(logfile, "SL", t_sender.SL_log);
    flush(logfile, "SR", t_sender.SR_log);
}

int main(int argc, char *argv[])
{

    if (argc != 8)
    {
        std::cerr << "Usage: " << argv[0] << " <taskfile> <logfile> <self_ip> <target_ip> <self_port> <target_port> <ghz>" << std::endl;
        return 1;
    }

    std::string taskfile = argv[1];
    std::string logfile = argv[2];
    std::string self_ip = argv[3];

    std::string target_ip = argv[4];
    int self_port = std::atoi(argv[5]);
    int target_port = std::atoi(argv[6]);

    double ghz = std::stod(argv[7]);

    std::vector<entry> send_entries;
    std::vector<long long> intvals;
    parse(taskfile, send_entries);

    for (int i = 0; i < send_entries.size(); i++)
    {
        long long nanoseconds;
        if (i == 0)
        {
            nanoseconds = (long long)send_entries[i].timestamp;
        }
        else
        {
            nanoseconds = (long long)(send_entries[i].timestamp - send_entries[i - 1].timestamp);
        }
        intvals.push_back(nanoseconds);
    }

    std::thread t(thread_function, send_entries, intvals, self_ip, target_ip, target_port, logfile, ghz);
    t.join();

    return 0;
}