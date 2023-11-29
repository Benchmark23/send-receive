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
    std::map<std::string, Log> RL_log;
    void init_log(std::vector<entry> &entries);
    void cycle_to_time(long long start_timestamp, uint64_t start_cycle, double ghz);

    virtual int accept__(int port) = 0;
    virtual int receive__() = 0;
    virtual void disconnect__() = 0;
};

class TCPReceiver : public Receiver
{
public:
    int accept__(int port);
    int receive__();
    void disconnect__();

private:
    int connect_socket;
    int server_socket;
};

class UDPReceiver : public Receiver
{
public:
    int accept__(int port);
    int receive__();
    void disconnect__();

private:
    int server_socket;
    struct sockaddr_in server_addr;
};

class RDMAReceiver : public Receiver
{
public:
    int accept__(int port);
    int receive__();
    void disconnect__();
};
