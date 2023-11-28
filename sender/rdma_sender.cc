#include "sender.h"

int RDMASender::connect__(std::string dst_ip, int port)
{
    return 0;
}

void RDMASender::send__(std::string id, int len)
{
    // std::cout << "Sending data using RDMA API" << std::endl;
}

void RDMASender::disconnect__()
{
}