#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "utils.h"
#include "tscns.h"

class Sender
{
public:
    TSCNS *tscns;
    std::string ip;
    std::map<std::string, Log> SL_log;
    std::map<std::string, Log> SR_log;
    void init_log(std::vector<entry> &entries);
    void cycle_to_time(long long start_timestamp, uint64_t start_cycle, double ghz);
    void set_timens(std::string id, std::map<std::string, Log> &logs);

    // TODO: require a unified structure rather than specifying the ip and port
    virtual int connect__(std::string dst_ip, int port) = 0;
    virtual int send__(std::string id, int len) = 0;
    virtual void disconnect__() = 0;
};

class TCPSender : public Sender
{
public:
    int connect__(std::string dst_ip, int port);
    int send__(std::string id, int len);
    void disconnect__();

private:
    int client_socket;
};

class UDPSender : public Sender
{
public:
    int connect__(std::string dst_ip, int port);
    int send__(std::string id, int len);
    void disconnect__();

private:
    int client_socket;
    struct sockaddr_in server_addr;
};

class RDMASender : public Sender
{
public:
    int connect__(std::string dst_ip, int port);
    int send__(std::string id, int len);
    void disconnect__();
};
