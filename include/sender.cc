#include "sender.h"

void SocketSender::send_message(std::string dst_ip,int port,std::string message){

    // std::cout << "Sending data using socket API" << std::endl;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating client socket" << std::endl;
        return ;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); 
    server_addr.sin_addr.s_addr = inet_addr(dst_ip.c_str()); 

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error connecting to receiver:"<<port<< std::endl;
        return ;
    }

    // 发送数据到服务器
    // std::string messages = "Hello from the client!";
    send(client_socket, message.c_str(), message.size(), 0);

    close(client_socket);
    return;
}


void RDMASender::send_message(std::string dst_ip,int port,std::string message){
    // std::cout << "Sending data using RDMA API" << std::endl;
}