#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "receiver.h"
#include "sender.h"

int main() {
    
    TCPReceiver receiver;
    int connect_socket,server_socket;
    receiver.accept__(connect_socket,server_socket,8081);
    receiver.receive__(connect_socket);
    receiver.disconnect__(connect_socket,server_socket);
    exit(0);

    return 0;
}