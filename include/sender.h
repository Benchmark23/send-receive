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

class Sender
{
public:
    std::string ip;
    std::map<std::string, log> SL_log;
    std::map<std::string, log> SR_log;
    void init_log(std::vector<entry> &entries);
    void cycle_to_time(long long start_timestamp, uint64_t start_cycle, int hz);

    // TODO: require a unified structure rather than specifying the ip and port
    virtual int connect__(std::string dst_ip, int port) = 0;
    virtual void send__(std::string id, int len) = 0;
    virtual void disconnect__() = 0;
};

class TCPSender : public Sender
{
public:
    int connect__(std::string dst_ip, int port);
    void send__(std::string id, int len);
    void disconnect__();

private:
    int client_socket;
};

class UDPSender : public Sender
{
public:
    int connect__(std::string dst_ip, int port);
    void send__(std::string id, int len);
    void disconnect__();

private:
    int client_socket;
    struct sockaddr_in server_addr;
};

class RDMASender : public Sender
{
public:
    int connect__(std::string dst_ip, int port);
    void send__(std::string id, int len);
    void disconnect__();
};
