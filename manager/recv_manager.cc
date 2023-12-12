#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "receiver.h"
#include "sender.h"

void thread_function(std::vector<entry> recv_entries,
                     std::string self_ip, int self_port, std::string logfile, int total_time, double ghz)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);

    TCPReceiver receiver;
    receiver.ip = self_ip;
    receiver.init_log(recv_entries);

    if (receiver.accept__(self_port) != 0)
    {
        return;
    }

    TSCNS tscns;
    tscns.init();
    receiver.tscns = &tscns;

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
        std::cerr << "Usage: " << argv[0] << " <taskfile> <logfile> <self_ip> <self_port> <total_time>" << std::endl;
        return 1;
    }

    std::string taskfile = argv[1];
    std::string logfile = argv[2];

    std::string self_ip = argv[3];
    int self_port = std::atoi(argv[4]);
    int total_time = std::atoi(argv[5]);

    // double ghz = std::stod(argv[6]);
    double ghz = 1.0;

    std::vector<entry> recv_entries;
    parse(taskfile, recv_entries);

    std::thread t(thread_function, recv_entries, self_ip, self_port, logfile, total_time + 3, ghz);
    t.join();

    return 0;
}