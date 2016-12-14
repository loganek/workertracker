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
#include <chrono>

namespace WT {

class DataContainer
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::chrono::seconds>> values;

public:
    void insert(const std::string &key, const std::string &value, std::chrono::seconds duration);

    std::chrono::seconds get_duration(const std::string &key, const std::string &value) const;

    std::vector<std::string> get_keys() const;
    std::vector<std::string> get_values(const std::string &key) const;

    void clear();
};

struct HourGroupPolicy
{
    static constexpr std::size_t array_size = 24;

    static std::size_t index(const DataEntry& entry)
    {
        return std::localtime(&entry.time_start)->tm_hour;
    }
};

struct WeekdayGroupPolicy
{
    static constexpr std::size_t array_size = 7;

    static std::size_t index(const DataEntry& entry)
    {
        return std::localtime(&entry.time_start)->tm_wday;
    }
};

class DataContainerV2 // TODO: remove DataContainer
{
    std::vector<DataEntry> entries;

public:
    virtual ~DataContainerV2() {}

    template <class... Args>
    void emplace_back(Args&&... args)
    {
        entries.emplace_back(std::forward<Args>(args)...);
    }

    template<typename GroupPolicy>
    std::array<std::pair<std::vector<const DataEntry*>, std::size_t>, GroupPolicy::array_size> get_grouped()
    {
        std::array<std::pair<std::vector<const DataEntry*>, std::size_t>, GroupPolicy::array_size> ret;

        for (const auto &entry : entries)
        {
            auto index = GroupPolicy::index(entry);
            ret[index].first.push_back(&entry);
            ret[index].second += entry.time_end - entry.time_start;
        }

        return ret;
    }
};

}

#endif // DATACONTAINER_H
