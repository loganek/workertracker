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

#include <tuple>
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace WT {

template<typename ConcretePolicy, int SIZE>
struct OfGroupPolicy
{
    using container_t = std::array<std::pair<std::time_t, int>, SIZE>;
    container_t container{};
    std::array<std::unordered_set<std::size_t>, SIZE> vals{};

    void process_entry(const DataEntry& entry)
    {
        auto time_st = std::localtime(&entry.time_start);
        auto key = ConcretePolicy::get_key(time_st);
        bool inserted;
        std::tie(std::ignore, inserted) = vals[key].insert(ConcretePolicy::get_count_key(time_st));

        container[key].first += entry.get_duration();
        container[key].second += inserted;
    }
};

struct HourGroupPolicy : public OfGroupPolicy<HourGroupPolicy, 24>
{
    static std::size_t get_key(const tm* time_st)
    {
        return time_st->tm_hour;
    }

    static std::size_t get_count_key(const tm* time_st)
    {
        return (time_st->tm_year << 8) | time_st->tm_yday;
    }
};

struct WeekdayGroupPolicy : public OfGroupPolicy<WeekdayGroupPolicy, 7>
{
    static std::size_t get_key(const tm* time_st)
    {
        return time_st->tm_wday;
    }

    static std::size_t get_count_key(const tm* time_st)
    {
        return (time_st->tm_year << 8) | time_st->tm_yday;
    }
};

struct ProcNameGroupPolicy
{
    using container_t = std::unordered_map<std::string, std::unordered_map<std::string, std::time_t>>;
    container_t container;

    void process_entry(const DataEntry& entry)
    {
        container[entry.proc_name][entry.description] += entry.get_duration();
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
