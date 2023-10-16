#include "utils.h"

void Utils::parse(const std::string& filename,std::vector<entry> &entries){
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
        // std::cout<<e.api<<std::endl;
        std::getline(file, line);
        e.size = std::stoi(line);
        entries.push_back(e);
        // skip empty line
        std::getline(file, line);
    }
}

void Utils::set_timestamp(std::string filename, std::string id){
     auto now = std::chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        long long timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

        std::ofstream file(filename, std::ios_base::app);
        if (file.is_open()) {
            file << id << " " << timestamp << "\n";
            file.close();
        }
}