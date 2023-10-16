#include "receiver.h"

void SocketReceiver::receive_message(int port){

    // std::cout << "Receiving data using socket API" << std::endl;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating server socket" << std::endl;
        return ;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // 使用端口8080
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error binding socket to address" << std::endl;
        return ;
    }

    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        return ;
    }

    std::cout << "receiver is listening on port "<<port<<"..." << std::endl;

    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        std::cerr << "Error accepting client connection" << std::endl;
        return ;
    }

    char buffer[256] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);
    std::cout << "sender says: " << buffer << std::endl;

    close(server_socket);
    close(client_socket);

    Utils recv;
    recv.set_timestamp("results/recv_result.log",buffer);
    return;
}

void RDMAReceiver::receive_message(int port){
    // std::cout << "Receiving data using RDMA API" << std::endl;
}