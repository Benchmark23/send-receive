#include "sender.h"

void TCPSender::connect__(int &client_socket, std::string dst_ip, int port)
{
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(dst_ip.c_str());
    server_addr.sin_port = htons(port);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Error connecting to receiver:" << port << std::endl;
    }
}

void TCPSender::send__(int &client_socket, std::string id, int len)
{
    std::string payload = id;
    payload.append(len - 36, '1');

    set_timestamp(id, SL_log);
    send(client_socket, id.c_str(), payload.size(), 0);
    set_timestamp(id, SR_log);
}

void TCPSender::disconnect__(int &client_socket)
{
    close(client_socket);
}