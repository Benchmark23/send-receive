#include "sender.h"

int TCPSender::connect__(std::string dst_ip, int port)
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(dst_ip.c_str());
    server_addr.sin_port = htons(port);

    // disable nagle's algorithm
    int flag = 1;
    int result = setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
    if (result == -1)
    {
        std::cerr << "Failed to set TCP_NODELAY option." << std::endl;
        return 1;
    }

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Error connecting to receiver. ip: " << dst_ip << " port: " << port << std::endl;
        return -1;
    }
    return 0;
}

void TCPSender::send__(std::string id, int len)
{
    std::string payload = id;
    payload.append(len - 36, '1');

    set_timestamp(id, SL_log);
    send(client_socket, payload.c_str(), payload.size(), 0);
    set_timestamp(id, SR_log);
}

void TCPSender::disconnect__()
{
    close(client_socket);
}