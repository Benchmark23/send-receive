#include "receiver.h"

void TCPReceiver::accept__(int &connect_socket,int &server_socket,int port){
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

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
    connect_socket = accept(server_socket, NULL, NULL);
    if (connect_socket == -1) {
        std::cerr << "Error accepting client connection" << std::endl;
        return ;
    }
}
void TCPReceiver::receive__(int &connect_socket){
    char buffer[256] = {0};
    recv(connect_socket, buffer, sizeof(buffer), 0);
    std::cout << "sender says: " << buffer << std::endl;
    return;
}
void TCPReceiver::disconnect__(int &connect_socket,int &server_socket){
    close(server_socket);
    close(connect_socket);
}