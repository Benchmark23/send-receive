#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

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
class Utils {
private:
public:
//    std::vector<entry> entries;
    void parse(const std::string& filename,std::vector<entry> &entries);
    void set_timestamp(std::string filename, std::string id);

};


#endif

