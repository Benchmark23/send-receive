#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "utils.h"

class Receiver
{
public:
    std::string ip;
    std::map<std::string, log> RL_log;
    void init_log(std::vector<entry> &entries);
    void cycle_to_time(long long start_timestamp, uint64_t start_cycle, int hz);
};

class TCPReceiver : public Receiver
{
public:
    void accept__(int &connect_socket, int &server_socket, int port);
    int receive__(int &connect_socket);
    void disconnect__(int &connect_socket, int &server_socket);
};

class UDPReceiver : public Receiver
{
public:
    void receive__(int port);
};

class RDMAReceiver : public Receiver
{
public:
    void receive__(int port);
};
