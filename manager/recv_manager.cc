#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "receiver.h"
#include "sender.h"

int main() {
    std::vector<entry> recv_entries;
    parse("assignment/task_receive.log",recv_entries);

    TCPReceiver receiver;
    int connect_socket,server_socket;
    receiver.accept__(connect_socket,server_socket,8081);

    int i=0;
    while(i<recv_entries.size()){
        if(receiver.receive__(connect_socket)==1){
            i++;
        }
    }
    receiver.disconnect__(connect_socket,server_socket);

    flush("results/result_recv",receiver.kvs);


    return 0;
}