#include "sender.h"

void Sender::init_log(std::vector<entry> &entries)
{
    for (int i = 0; i < entries.size(); i++)
    {
        Log sl_log_entry;
        sl_log_entry.timestamp = 0;
        sl_log_entry.ip = this->ip;
        sl_log_entry.port = entries[i].src_port;
        sl_log_entry.protocol = entries[i].api;
        sl_log_entry.len = entries[i].size;

        SL_log[entries[i].id] = sl_log_entry;
        SR_log[entries[i].id] = sl_log_entry;

        SL_log.insert(std::make_pair(entries[i].id, sl_log_entry));
        SR_log.insert(std::make_pair(entries[i].id, sl_log_entry));
    }
}

void Sender::cycle_to_time(long long start_timestamp, uint64_t start_cycle, double ghz)
{
    for (std::map<std::string, Log>::iterator it = SL_log.begin(); it != SL_log.end();)
    {
        long long cycle = it->second.timestamp;
        if (cycle == 0)
        {
            it = SL_log.erase(it);
        }
        else
        {
            double second = (double)(cycle - start_cycle) / ghz;
            it->second.timestamp = second + start_timestamp;
            it++;
        }
    }

    for (std::map<std::string, Log>::iterator it = SR_log.begin(); it != SR_log.end();)
    {
        long long cycle = it->second.timestamp;
        if (cycle == 0)
        {
            it = SR_log.erase(it);
        }
        else
        {
            double second = (double)(cycle - start_cycle) / ghz;
            it->second.timestamp = second  + start_timestamp;
            it++;
        }
    }
}