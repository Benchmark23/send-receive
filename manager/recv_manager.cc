#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "receiver.h"
#include "sender.h"
void thread_function(std::vector<entry> recv_entries,
    std::string self_ip , int self_port , std::string logfile, int total_time) {

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    sched_setaffinity(0, sizeof(mask), &mask);

    TCPReceiver receiver;
    receiver.ip=self_ip;
    receiver.init_log(recv_entries);

    int connect_socket,server_socket;
    receiver.accept__(connect_socket,server_socket,self_port);

    auto start_time = std::chrono::steady_clock::now();
    while(1){

        receiver.receive__(connect_socket);
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() > total_time) {  // 5 秒后停止
            break;
        }
    }

    receiver.disconnect__(connect_socket,server_socket);
    // receiver.cycle_to_time(1000000000);

    flush(logfile,"RL",receiver.RL_log);
}

int main(int argc, char *argv[]) {

    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <taskfile> <logfile> <self_ip> <self_port>" << std::endl;
        return 1;
    }

    std::string taskfile = argv[1];
    std::string logfile = argv[2];

    std::string self_ip = argv[3];
    int self_port = std::atoi(argv[4]);

    std::vector<entry> recv_entries;
    parse(taskfile, recv_entries);

    std::thread t(thread_function,recv_entries,self_ip,self_port,logfile,5);
    t.join();

    return 0;
}