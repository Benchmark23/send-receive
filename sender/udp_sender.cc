#include "sender.h"

void UDPSender::send__(std::string dst_ip,int port,std::string message){

    // std::cout << "Sending data using socket API" << std::endl;
    int client_socket = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(dst_ip.c_str()); 
    server_addr.sin_port = htons(port); 

    sendto(client_socket, message.c_str(), message.size(), 0,
      (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(client_socket);
    return;
}