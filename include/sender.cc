#include "sender.h"

void TCPSender::send_(std::string dst_ip,int port,std::string message){

    // std::cout << "Sending data using socket API" << std::endl;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(dst_ip.c_str()); 
    server_addr.sin_port = htons(port); 
    

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

void UDPSender::send_(std::string dst_ip,int port,std::string message){

    // std::cout << "Sending data using socket API" << std::endl;
    int client_socket = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(dst_ip.c_str()); 
    server_addr.sin_port = htons(port); 
    

    // 发送数据到服务器
    // std::string messages = "Hello from the client!";
    // send(client_socket, message.c_str(), message.size(), 0);
    sendto(client_socket, message.c_str(), message.size(), 0,
      (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(client_socket);
    return;
}

void RDMASender::send_(std::string dst_ip,int port,std::string message){
    // std::cout << "Sending data using RDMA API" << std::endl;
}