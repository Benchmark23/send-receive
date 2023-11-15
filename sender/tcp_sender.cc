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

    set_timestamp(id,SL_log);
    // send(client_socket, all.c_str(), all.size(), 0);
    send(client_socket, id.c_str(), id.size(), 0);
    set_timestamp(id,SR_log);
    return;
}
void TCPSender::disconnect__(int &client_socket){
    close(client_socket);
    return;
}

void TCPSender::init_log(std::vector<entry> &entries){

    for(int i = 0;i < entries.size(); i++){
        log sl_log_entry;
        sl_log_entry.timestamp=0;
        sl_log_entry.ip = this->ip;
        sl_log_entry.port = entries[i].src_port;
        sl_log_entry.protocol = entries[i].api;
        sl_log_entry.len = entries[i].size;

        SL_log[entries[i].id] = sl_log_entry;
        SR_log[entries[i].id] = sl_log_entry;

        SL_log.insert(std::make_pair(entries[i].id,sl_log_entry));
        SR_log.insert(std::make_pair(entries[i].id,sl_log_entry));
    }
}