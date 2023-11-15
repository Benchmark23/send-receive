#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "utils.h"

#ifndef SENDER_H
#define SENDER_H

class Sender {
public:
  std::string ip;
  std::map<std::string, log> SL_log;
  std::map<std::string, log> SR_log;
};

class TCPSender : public Sender {
public:
    void connect__(int &client_socket,std::string dst_ip,int port);
    void send__(int &client_socket,std::string id,int len);
    void disconnect__(int &client_socket);
    void init_log(std::vector<entry> &entries);

};

class UDPSender : public Sender {
public:
    void send__(std::string dst_ip,int port,std::string message);
};

class RDMASender : public Sender {
public:
    void send__(std::string dst_ip,int port,std::string message);
};


#endif
