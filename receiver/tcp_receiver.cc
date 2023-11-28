#include "receiver.h"

int TCPReceiver::accept__(int port)
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Error binding socket to address" << std::endl;
        return -1;
    }
    if (listen(server_socket, 5) == -1)
    {
        std::cerr << "Error listening for connections" << std::endl;
        return -1;
    }
    std::cout << "receiver is listening on port " << port << "..." << std::endl;
    connect_socket = accept(server_socket, nullptr, nullptr);
    if (connect_socket == -1)
    {
        std::cerr << "Error accepting client connection" << std::endl;
        return -1;
    }
    return 0;
}

int TCPReceiver::receive__()
{
    char buffer[2048] = {0};
    auto bytes = recv(connect_socket, buffer, sizeof(buffer), 0);

    if (bytes <= 0)
    {
        return -1;
    }

    std::string extractedString(buffer, 36);
    set_timestamp(extractedString, RL_log);
    return 0;
}

void TCPReceiver::disconnect__()
{
    close(server_socket);
    close(connect_socket);
}
