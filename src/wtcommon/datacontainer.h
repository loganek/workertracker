/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include "dataentry.h"

#include <vector>
#include <unordered_map>

namespace WT {

struct HourGroupPolicy
{
    using container_t = std::array<std::size_t, 24>;
    container_t container{};

    void process_entry(const DataEntry& entry)
    {
        container[std::localtime(&entry.time_start)->tm_hour] += entry.get_duration();
    }
};

struct WeekdayGroupPolicy
{
    using container_t = std::array<std::size_t, 7>;
    container_t container{};

    void process_entry(const DataEntry& entry)
    {
        container[std::localtime(&entry.time_start)->tm_wday] += entry.get_duration();
    }
};

struct ProcNameGroupPolicy
{
    using container_t = std::unordered_map<std::string, std::vector<const DataEntry*>>;
    container_t container;

    void process_entry(const DataEntry& entry)
    {
        container[entry.proc_name].push_back(&entry);
    }
};

class DataContainer
{
    std::vector<DataEntry> entries;

public:
    virtual ~DataContainer() {}

    template <class... Args>
    void emplace_back(Args&&... args)
    {
        entries.emplace_back(std::forward<Args>(args)...);
    }

    template<typename GroupPolicy>
    typename GroupPolicy::container_t get_grouped() const
    {
        GroupPolicy policy;

        for (const auto &entry : entries)
        {
            policy.process_entry(entry);
        }

        return policy.container;
    }

};

}

#endif // DATACONTAINER_H
