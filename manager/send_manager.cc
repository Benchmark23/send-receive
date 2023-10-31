#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include "utils.h"
#include "receiver.h"
#include "sender.h"
int main(int argc, char *argv[])
{

    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0] << " <taskfile> <logfile> <target_ip> <self_port> <target_port>" << std::endl;
        return 1;
    }

    std::string taskfile = argv[1];
    std::string logfile = argv[2];
    std::string target_ip = argv[3];

    int self_port = std::atoi(argv[4]);
    int target_port = std::atoi(argv[5]);

    std::vector<entry> send_entries;
    std::vector<long long> intvals;
    parse(taskfile, send_entries);

    for(int i = 0; i < send_entries.size(); i++){
        long long nanoseconds;
        if(i==0){
            nanoseconds=0;
        }
        else{
            nanoseconds=(long long)(send_entries[i].timestamp-send_entries[i-1].timestamp);
        }
        intvals.push_back(nanoseconds);
    }

    TCPSender t_sender;
    int t_client_socket=0;
    UDPSender u_sender;
    
    t_sender.connect__(t_client_socket, target_ip, target_port);

    for(int i=0;i<send_entries.size();i++){

        std::this_thread::sleep_for(std::chrono::nanoseconds(intvals[i]));
        t_sender.send__(t_client_socket,send_entries[i].id,send_entries[i].size);
        // if(send_entries[i].api=="tcp"){
        //     if(i!=0&&send_entries[i].dst!=send_entries[i-1].dst){
        //         t_sender.disconnect__(t_client_socket);
        //     }
        //     if(t_client_socket==0){
        //         t_sender.connect__(t_client_socket,"127.0.0.1",8081);
        //     }
        //     std::this_thread::sleep_for(std::chrono::nanoseconds(intvals[i]));
        //     t_sender.send__(t_client_socket,send_entries[i].id);
        // }
        // else if (send_entries[i].api=="udp"){
        //     t_sender.disconnect__(t_client_socket);
        //     std::this_thread::sleep_for(std::chrono::nanoseconds(intvals[i]));
        //     u_sender.send__("127.0.0.1",8081,send_entries[i].id);
        // }
    }

    t_sender.disconnect__(t_client_socket);

    flush(logfile,"SL", t_sender.SL_log);
    flush(logfile,"SR", t_sender.SR_log);
    
    return 0;
}