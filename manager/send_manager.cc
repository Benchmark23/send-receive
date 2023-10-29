#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "receiver.h"
#include "sender.h"
int main() {
   
    TCPSender sender;
    int client_socket;
    std::chrono::nanoseconds sleep_time(1000000000);
    std::this_thread::sleep_for(sleep_time);
    sender.connect__(client_socket,"127.0.0.1",8081);
    sender.send__(client_socket,"111");
    sender.disconnect__(client_socket);

    return 0;
}