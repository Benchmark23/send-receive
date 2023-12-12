#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include "utils.h"
#include "receiver.h"
#include "sender.h"
#include "tscns.h"

void thread_function(std::vector<entry> send_entries, std::string self_ip,
                     std::string target_ip, int target_port, std::string logfile)
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

    TSCNS tscns;
    tscns.init();
    t_sender.tscns = &tscns;

    auto start_cycle = tscns.rdtsc();
    std::cerr << "start_cycle: " << start_cycle << std::endl;
    std::cerr << "start_timestamp: " << tscns.tsc2ns(start_cycle) << std::endl;

    int i = 0;
    int upper = send_entries.size();
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

    if (argc != 7)
    {
        std::cerr << "Usage: " << argv[0] << " <taskfile> <logfile> <self_ip> <target_ip> <self_port> <target_port>" << std::endl;
        return 1;
    }

    std::string taskfile = argv[1];
    std::string logfile = argv[2];
    std::string self_ip = argv[3];

    std::string target_ip = argv[4];
    int self_port = std::atoi(argv[5]);
    int target_port = std::atoi(argv[6]);

    std::vector<entry> send_entries;
    parse(taskfile, send_entries);

    std::thread t(thread_function, send_entries, self_ip, target_ip, target_port, logfile);
    t.join();

    return 0;
}