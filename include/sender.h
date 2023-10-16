#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#ifndef SENDER_H
#define SENDER_H

class Sender {
public:
    virtual void send_message(std::string dst_ip,int port,std::string message) = 0;
};


class SocketSender : public Sender {
public:
    void send_message(std::string dst_ip,int port,std::string message) override;

};

class RDMASender : public Sender {
public:
    void send_message(std::string dst_ip,int port,std::string message) override;
};


#endif
