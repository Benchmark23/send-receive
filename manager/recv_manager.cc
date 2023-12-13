#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "receiver.h"
#include "sender.h"

void thread_function(std::vector<entry> recv_entries, std::string logfile, int cpu_id, int64_t start_time, int total_time)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_id, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);

    std::string self_ip = recv_entries[0].dst_ip;
    int self_port = recv_entries[0].dst_port;

    TCPReceiver receiver;
    receiver.ip = self_ip;
    receiver.init_log(recv_entries);

    TSCNS tscns;
    tscns.init();
    receiver.tscns = &tscns;

    if (receiver.accept__(self_port) != 0)
    {
        return;
    }

    // while (tscns.rdns() < start_time)
    // {
    //     // wait to the start time
    // }

    auto begin_time = std::chrono::steady_clock::now();
    while (1)
    {
        if (receiver.receive__() != 0)
        {
            break;
        }
        // TODO: use a better method to control when to stop the receiver
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - begin_time).count() > total_time)
        {
            break;
        }
    }

    receiver.disconnect__();

    flush(logfile, "RL", receiver.RL_log);
}

int main(int argc, char *argv[])
{

    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0] << " <taskfile> <logfile> <cpu_id> <start_time> <total_time>" << std::endl;
        return 1;
    }

    std::string taskfile = argv[1];
    std::string logfile = argv[2];

    int cpu_id = std::atoi(argv[3]);
    int64_t start_time = std::atoi(argv[4]);
    int total_time = std::atoi(argv[5]);

    std::vector<entry> recv_entries;
    parse(taskfile, recv_entries);

    std::thread t(thread_function, recv_entries, logfile, cpu_id, start_time, total_time + 3);
    t.join();

    return 0;
}