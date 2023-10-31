#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "receiver.h"
#include "sender.h"

int main(int argc, char *argv[]) {

    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <taskfile> <logfile> <self_port>" << std::endl;
        return 1;
    }

    std::string taskfile = argv[1];
    std::string logfile = argv[2];

    int self_port = std::atoi(argv[3]);

    std::vector<entry> recv_entries;
    parse(taskfile, recv_entries);

    

    TCPReceiver receiver;
    int connect_socket,server_socket;
    receiver.accept__(connect_socket,server_socket,self_port);

    int i=0;
    while(i<recv_entries.size()){
        if(receiver.receive__(connect_socket)==1){
            i++;
            std::cout<<i<<std::endl;
        }
    }
    receiver.disconnect__(connect_socket,server_socket);

    flush(logfile,"RL",receiver.RL_log);

    return 0;
}