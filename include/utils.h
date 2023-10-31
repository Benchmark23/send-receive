#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <sched.h>

#ifndef UTILS_H
#define UTILS_H

struct entry{
    std::string id;
    long long timestamp;

    std::string src_ip;
    std::string dst_ip;
    
    int src_port;
    int dst_port;
    
    std::string api;
    int size;
};

struct log{
    // std::string id;
    // int cpu;
    // uint64_t tsc;
    std::string id;
    long long timestamp;
    std::string ip;
    int port;
    std::string protocol;
    int len;

};
static inline uint64_t rdtsc(void)
{
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}
static inline void parse(const std::string& filename,std::vector<entry> &entries){
      entries.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        entry e;
        std::stringstream ss(line);
        std::string type;
        ss>>type;

        ss>>e.id;
        // std::cout<<e.id<<std::endl;
        ss>>e.timestamp;
        // std::cout<<e.timestamp<<std::endl;
        ss>>e.src_ip;
        // std::cout<<e.src_ip<<std::endl;
        ss>>e.dst_ip;
        // std::cout<<e.dst_ip<<std::endl;
        ss>>e.src_port;
        // std::cout<<e.src_port<<std::endl;
        ss>>e.dst_port;
        // std::cout<<e.dst_port<<std::endl;
        ss>>e.api;
        // std::cout<<e.api<<std::endl;
        ss>>e.size;
        // std::cout<<e.size<<std::endl;
        entries.push_back(e);
    }
}
static inline void set_timestamp(std::string id,std::string ip,int port,std::string protocol,int len,std::vector<log> &logs){
    
    log tmp;
    tmp.id=id;
    // tmp.cpu = sched_getcpu();
    // tmp.tsc = rdtsc();
    tmp.ip=ip;
    tmp.port=port;
    tmp.protocol=protocol;
    tmp.len=len;

    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    long long timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    tmp.timestamp=timestamp;

    logs.push_back(tmp);

}

static inline void flush(std::string filename,std::string log_type,std::vector<log> logs){
    std::ofstream file(filename, std::ios_base::app);
        if (file.is_open()) {
            for(int i = 0;i < logs.size(); i++){
                file <<log_type<<" "<<logs[i].id<<" "<<logs[i].timestamp<<" "
                <<logs[i].ip<<" "<<logs[i].port<<" "<<logs[i].protocol<<" "<<logs[i].len<<"\n";
            }
            file.close();
        }
}


#endif

