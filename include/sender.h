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
    void cycle_to_time(long long start, int hz);
};

class TCPSender : public Sender
{
public:
    void connect__(int &client_socket, std::string dst_ip, int port);
    void send__(int &client_socket, std::string id, int len);
    void disconnect__(int &client_socket);
};

class UDPSender : public Sender
{
public:
    void send__(std::string dst_ip, int port, std::string message);
};

class RDMASender : public Sender
{
public:
    void send__(std::string dst_ip, int port, std::string message);
};
