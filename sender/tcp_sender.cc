#include "sender.h"

void TCPSender::connect__(int &client_socket,std::string dst_ip,int port){

    // std::cout << "Sending data using socket API" << std::endl;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(dst_ip.c_str()); 
    server_addr.sin_port = htons(port); 
    
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error connecting to receiver:"<<port<< std::endl;
        return ;
    }
    return;
}
void TCPSender::send__(int &client_socket,std::string message){
    // std::cout << "Sending data using socket API" << std::endl;
    send(client_socket, message.c_str(), message.size(), 0);
    return;
}
void TCPSender::disconnect__(int &client_socket){
    close(client_socket);
    return;
}