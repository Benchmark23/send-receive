#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "receiver.h"
#include "sender.h"
using namespace std;

int main() {
    Utils receive_util;
    Utils send_util;
    std::vector<entry> send_entries;
    std::vector<entry> recv_entries;
    receive_util.parse("assignment/task_receive.log",recv_entries);
    send_util.parse("assignment/task_send.log",send_entries);
    
    // cout<<recv_entries.size()<<send_entries.size();

    for (int i = 0; i < recv_entries.size(); ++i) {
        pid_t pid = fork();
        if (pid == 0) { // child process
            if(recv_entries[i].api=="tcp"){
                SocketReceiver receiver;
                receiver.receive_message(8081+i);
            }
            exit(0);
        }
    }
    
    for (int i = 0; i < send_entries.size(); ++i) {
        pid_t pid = fork();
        if (pid == 0) { // child process
            if(send_entries[i].api=="tcp"){
                SocketSender sender;

                long long nanoseconds = (long long)(send_entries[i].timestamp* 1000000000);
                std::chrono::nanoseconds sleep_time(nanoseconds);
                std::this_thread::sleep_for(sleep_time);

                send_util.set_timestamp("results/send_result.log",send_entries[i].id);

                sender.send_message(send_entries[i].dst,8081+i,send_entries[i].id);
            }
            exit(0);
        }
    }
    return 0;
}