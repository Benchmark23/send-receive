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
void TCPSender::send__(int &client_socket,std::string id,int len){
    // std::cout << "Sending data using socket API" << std::endl;
    std::string all = id;
    all.append(len-36,'1');

    set_timestamp(id,"127.0.0.1",8080,"tcp",len,SL_log);
    // send(client_socket, all.c_str(), all.size(), 0);
    send(client_socket, id.c_str(), id.size(), 0);
    set_timestamp(id,"127.0.0.1",8080,"tcp",len,SR_log);
    return;
}
void TCPSender::disconnect__(int &client_socket){
    close(client_socket);
    return;
}