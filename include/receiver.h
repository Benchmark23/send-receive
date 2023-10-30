#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "utils.h"

#ifndef RECEIVER_H
#define RECEIVER_H

class Receiver {
public:
   std::vector<kv> kvs;
};

class TCPReceiver : public Receiver {
public:
    void accept__(int &connect_socket,int &server_socket,int port);
    int receive__(int &connect_socket);
    void disconnect__(int &connect_socket,int &server_socket);
};

class UDPReceiver : public Receiver {
public:
    void receive__(int port);
};

class RDMAReceiver : public Receiver {
public:
    void receive__(int port);
};

#endif