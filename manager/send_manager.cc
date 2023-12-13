#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include "utils.h"
#include "receiver.h"
#include "sender.h"
#include "tscns.h"

void thread_function(std::vector<entry> send_entries, std::string logfile, int cpu_id, int64_t start_time)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_id, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);

    std::string self_ip = send_entries[0].src_ip;
    std::string target_ip = send_entries[0].dst_ip;
    int target_port = send_entries[0].dst_port;

    TCPSender t_sender;
    t_sender.ip = self_ip;
    t_sender.init_log(send_entries);

    UDPSender u_sender;

    int i = 0;
    int upper = send_entries.size();

    TSCNS tscns;
    tscns.init();
    t_sender.tscns = &tscns;

    while(tscns.rdns() < start_time)
    {
        // wait to the start time
    }

    if (t_sender.connect__(target_ip, target_port) != 0)
    {
        return;
    }

    auto start_cycle = tscns.rdtsc();
    while (i < upper)
    {
        auto now_cycle = tscns.rdtsc();
        if (tscns.tsc2ns(now_cycle) - tscns.tsc2ns(start_cycle) >= send_entries[i].timestamp)
        {
            auto send_res = t_sender.send__(send_entries[i].id, send_entries[i].size);
            if (send_res < 0)
            {
                std::cerr << "send error" << std::endl;
                break;
            }
            i++;
        }
        else if (tscns.tsc2ns(now_cycle + 10000000) < send_entries[i].timestamp)
        {
            tscns.calibrate();
        }
    }

    t_sender.disconnect__();

    flush(logfile, "SL", t_sender.SL_log);
    flush(logfile, "SR", t_sender.SR_log);
}

int main(int argc, char *argv[])
{

    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <taskfile> <logfile> <cpu_id> <start_time>" << std::endl;
        return 1;
    }

    std::string taskfile = argv[1];
    std::string logfile = argv[2];

    int cpu_id = std::atoi(argv[3]);
    int64_t start_time = std::atoi(argv[4]);

    std::vector<entry> send_entries;
    parse(taskfile, send_entries);

    std::thread t(thread_function, send_entries, logfile, cpu_id, start_time);
    t.join();

    return 0;
}