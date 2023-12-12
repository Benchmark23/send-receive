#include "receiver.h"

void Receiver::init_log(std::vector<entry> &entries)
{
    for (int i = 0; i < entries.size(); i++)
    {
        Log rl_log_entry;
        rl_log_entry.timestamp = 0;
        rl_log_entry.ip = this->ip;
        rl_log_entry.port = entries[i].dst_port;
        rl_log_entry.protocol = entries[i].api;
        rl_log_entry.len = entries[i].size;

        RL_log.insert(std::make_pair(entries[i].id, rl_log_entry));
    }
}

void Receiver::cycle_to_time(long long start_timestamp, uint64_t start_cycle, double ghz)
{
    for (std::map<std::string, Log>::iterator it = RL_log.begin(); it != RL_log.end();)
    {
        long long cycle = it->second.timestamp;
        if (cycle == 0)
        {
            it = RL_log.erase(it);
        }
        else
        {
            double nanoseconds = (double)(cycle - start_cycle) / ghz;
            it->second.timestamp = nanoseconds + start_timestamp;
            ++it;
        }
    }
}

void Receiver::set_timens(std::string id, std::map<std::string, Log> &logs)
{
    auto iterator = logs.find(id);
    if (iterator == logs.end())
    {
        std::cerr << "invalid id: " << id << std::endl;
    }
    else
    {
        iterator->second.timestamp = tscns->rdns();
    }
}