#include "receiver.h"

void UDPReceiver::receive__(int port){

    // std::cout << "Receiving data using socket API" << std::endl;
    int server_socket = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // 使用端口8080
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error binding socket to address" << std::endl;
        return ;
    }

    std::cout << "receiver is listening on port "<<port<<"..." << std::endl;

    char buffer[256] = {0};
    recvfrom(server_socket, buffer, sizeof(buffer), 0,NULL,0);
    std::cout << "sender says: " << buffer << std::endl;

    close(server_socket);

    // Utils recv;
    // recv.set_timestamp("results/recv_result.log",buffer);
    return;
}