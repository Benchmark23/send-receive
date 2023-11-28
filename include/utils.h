#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <sched.h>

struct entry
{
    std::string id;
    long long timestamp;

    std::string src_ip;
    std::string dst_ip;

    int src_port;
    int dst_port;

    std::string api;
    int size;
};

struct Log
{
    long long timestamp;
    std::string ip;
    int port;
    std::string protocol;
    int len;
};

static inline uint64_t rdtsc(void)
{
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

static inline void parse(const std::string &filename, std::vector<entry> &entries)
{
    entries.clear();
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        entry e;
        std::stringstream ss(line);
        std::string type;

        ss >> type;
        ss >> e.id;
        ss >> e.timestamp;
        ss >> e.src_ip;
        ss >> e.dst_ip;
        ss >> e.src_port;
        ss >> e.dst_port;
        ss >> e.api;
        ss >> e.size;
        entries.push_back(e);
    }
}
static inline void set_timestamp(std::string id, std::map<std::string, Log> &logs)
{
    auto iterator = logs.find(id);
    if (iterator == logs.end())
    {
        std::cerr << "invalid id: " << id << std::endl;
    }
    iterator->second.timestamp = rdtsc();
}

static inline void flush(std::string filename, std::string log_type, std::map<std::string, Log> &logs)
{
    std::ofstream file(filename, std::ios_base::app);
    if (file.is_open())
    {
        for (std::map<std::string, Log>::iterator it = logs.begin(); it != logs.end(); it++)
        {

            file << log_type << " " << it->first << " " << it->second.timestamp << " "
                 << it->second.ip << " " << it->second.port << " " << it->second.protocol << " " << it->second.len
                 << "\n";
        }
        file.close();
    }
}
