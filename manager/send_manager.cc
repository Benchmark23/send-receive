#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include "utils.h"
#include "receiver.h"
#include "sender.h"

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

    auto start_cycle = rdtsc();
    auto start_time = std::chrono::high_resolution_clock::now();
    auto duration = start_time.time_since_epoch();
    long long start_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

    // 
    int i = 0;
    int upper = send_entries.size();
    while(i < upper){
        long long now_cycle = rdtsc();
        long long now_time = (double)(now_cycle - start_cycle) / ghz;
        if(now_time >= send_entries[i].timestamp){
            if(t_sender.send__(send_entries[i].id, send_entries[i].size) < 0){
               std::cerr << "send error" << std::endl;
               break;
            }
            i++;
        }
    }

    t_sender.disconnect__();

    t_sender.cycle_to_time(start_timestamp, start_cycle, ghz);

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