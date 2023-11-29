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

void Receiver::cycle_to_time(long long start_timestamp, uint64_t start_cycle, int hz)
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
            double second = (double)(cycle - start_cycle) / (double)hz;
            it->second.timestamp = second * 1000000000 + start_timestamp;
            ++it;
        }
    }
}