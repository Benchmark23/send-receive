#include "receiver.h"

int UDPReceiver::accept__(int port)
{
    server_socket = socket(PF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cerr << "Error binding socket to address" << std::endl;
        return -1;
    }

    std::cout << "receiver is listening on port " << port << "..." << std::endl;
    return 0;
}

int UDPReceiver::receive__()
{
    char buffer[2048] = {0};
    auto bytes = recvfrom(server_socket, buffer, sizeof(buffer), 0, nullptr, 0);

    if (bytes <= 0)
    {
        return -1;
    }

    std::string extractedString(buffer, 36);
    set_timestamp(extractedString, RL_log);
    return 0;
}

void UDPReceiver::disconnect__()
{
    close(server_socket);
}