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
    int32_t payload_size;
    int bytes_received = recv(connect_socket, reinterpret_cast<char *>(&payload_size), sizeof(payload_size), 0);

    if (bytes_received <= 0)
    {
        if (bytes_received == 0)
        {
            return 0;
        }
        else
        {
            std::cerr << "<payload_size> Error during recv.\n";
            return -1;
        }
    }

    std::vector<char> buffer;
    buffer.reserve(payload_size);
    int total_bytes_received = 0;

    while (total_bytes_received < payload_size)
    {
        bytes_received = recv(connect_socket, buffer.data() + total_bytes_received, payload_size - total_bytes_received, 0);

        if (bytes_received <= 0)
        {
            if (bytes_received == 0)
            {
                std::cerr << "<buffer> Connection closed by peer.\n";
            }
            else
            {
                std::cerr << "<buffer> Error during recv.\n";
            }
            return -1;
        }
        total_bytes_received += bytes_received;
    }

    std::string extractedString(buffer.data(), 36);
    set_timestamp(extractedString, RL_log);
    return 0;
}

void TCPReceiver::disconnect__()
{
    close(server_socket);
    close(connect_socket);
}
