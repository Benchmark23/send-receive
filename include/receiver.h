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
    virtual void receive_(int port) = 0;
};

class TCPReceiver : public Receiver {
public:
    void receive_(int port) override;
};

class UDPReceiver : public Receiver {
public:
    void receive_(int port) override;
};

class RDMAReceiver : public Receiver {
public:
    void receive_(int port) override;
};

#endif