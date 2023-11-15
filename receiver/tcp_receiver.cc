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
int TCPReceiver::receive__(int &connect_socket){
    char buffer[2048] = {0};
    size_t bytes = recv(connect_socket, buffer, sizeof(buffer), 0);
   
    if(bytes!=0){
        std::string extractedString(buffer, 36);
        set_timestamp(extractedString,RL_log);
        // std::cout << "sender says: " << buffer << std::endl;
        return 1;
    }
    return 0;
}
void TCPReceiver::disconnect__(int &connect_socket,int &server_socket){
    close(server_socket);
    close(connect_socket);
}


void TCPReceiver::init_log(std::vector<entry> &entries){
    for(int i = 0;i < entries.size(); i++){
        log rl_log_entry;
        rl_log_entry.timestamp=0;
        rl_log_entry.ip = this->ip;
        rl_log_entry.port = entries[i].dst_port;
        rl_log_entry.protocol = entries[i].api;
        rl_log_entry.len = entries[i].size;

        // RL_log[entries[i].id] = rl_log_entry;
        RL_log.insert(std::make_pair(entries[i].id,rl_log_entry));
        
    }
}
void TCPReceiver::cycle_to_time(int hz){
    for(std::map<std::string, log>::iterator it=RL_log.begin();it!=RL_log.end();it++){

        long long cycle = it->second.timestamp;
        double second = cycle / hz;
        it->second.timestamp = second * 1000000000 ;
    }
}