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
    std::string src;
    std::string dst;
    double timestamp;
    std::string api;
    std::string id;
    int size;
};

struct kv{
    std::string id;
    int cpu;
    uint64_t tsc;
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
        e.src = line.substr(0, line.find(' '));
        e.dst = line.substr(line.find(' ') + 1);
        // std::cout<<e.src<<" "<<e.dst<<std::endl;
        std::getline(file, line);
        e.timestamp = std::stod(line);
        // std::cout<<e.timestamp<<std::endl;
        std::getline(file, e.api);
        // std::cout<<e.api<<std::endl;
        std::getline(file, e.id);
        // std::cout<<e.id<<std::endl;
        std::getline(file, line);
        e.size = std::stoi(line);
        entries.push_back(e);
        std::getline(file, line);
    }
}
static inline void set_timestamp(std::string id,std::vector<kv> &kvs){
     kv tmp;
    tmp.id=id;
    tmp.cpu = sched_getcpu();
    tmp.tsc = rdtsc();
    kvs.push_back(tmp);
}

static inline void flush(std::string filename,std::vector<kv> kvs){
    std::ofstream file(filename, std::ios_base::app);
        if (file.is_open()) {

            for(int i = 0;i < kvs.size(); i++){
                file << kvs[i].id<< kvs[i].cpu <<" " << kvs[i].tsc << "\n";
            }
            file.close();
        }
}


#endif

