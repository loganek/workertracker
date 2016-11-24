/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "datacontainer.h"

#include <algorithm>

namespace WT {

void DataContainer::insert(const std::string &key, const std::string &value, std::chrono::seconds duration)
{
    if (values.find(key) != values.end())
    {
        auto& m = values[key];
        if (m.find(value) != m.end())
        {
            m[value] += duration;
        }
        else
        {
            m[value] = duration;
        }
    }
    else
    {
        values[key][value] = duration;
    }
}

std::chrono::seconds DataContainer::get_duration(const std::string &key, const std::string &value) const
{
    return values.at(key).at(value);
}

std::chrono::seconds DataContainer::get_duration(const std::string &key, boost::optional<std::regex> pattern) const
{
    const std::unordered_map<std::string, std::chrono::seconds>& c = values.at(key);
    std::chrono::seconds total = std::chrono::seconds(0);

    for (const auto& entry : c)
    {
        if (!pattern || std::regex_search(entry.first, pattern.get()))
        {
            total += entry.second;
        }
    }

    return total;
}

std::vector<std::string> DataContainer::get_keys() const
{
    std::vector<std::string> keys;

    for (const auto& k : values)
    {
        keys.push_back(k.first);
    }

    return keys;
}

std::vector<std::string> DataContainer::get_values(const std::string &key) const
{
    std::vector<std::string> vals;

    for (const auto& v : values.at(key))
    {
        vals.push_back(v.first);
    }

    return vals;
}

void DataContainer::clear()
{
    values.clear();
}

}
