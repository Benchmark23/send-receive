#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include "utils.h"
#include "receiver.h"
#include "sender.h"
int main() {

    Utils send_util;
    std::vector<entry> send_entries;
    std::vector<long long> intvals;
    send_util.parse("assignment/task_send.log",send_entries);

    for(int i = 0; i < send_entries.size(); i++){
        long long nanoseconds;
        if(i==0){
            nanoseconds=0;
        }
        else{
            nanoseconds=(long long)((send_entries[i].timestamp-send_entries[i-1].timestamp)*1000000000);
        }
        intvals.push_back(nanoseconds);
    }

    TCPSender sender;
    int client_socket;
    std::chrono::nanoseconds sleep_time(1000000000);
    sender.connect__(client_socket,"127.0.0.1",8081);
    for(int i=0;i<send_entries.size();i++){
        if(send_entries[i].api=="tcp"){
        }
        else if (send_entries[i].api=="udp"){
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(intvals[i]));
        sender.send__(client_socket,send_entries[i].id);
        
    }
    sender.disconnect__(client_socket);

    return 0;
}