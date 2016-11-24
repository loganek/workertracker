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

#include <boost/optional.hpp>

#include <vector>
#include <unordered_map>
#include <chrono>
#include <regex>

namespace WT {

class DataContainer
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::chrono::seconds>> values;

public:
    void insert(const std::string &key, const std::string &value, std::chrono::seconds duration);

    std::chrono::seconds get_duration(const std::string &key, const std::string &value) const;
    std::chrono::seconds get_duration(const std::string &key, boost::optional<std::regex> pattern = boost::none) const;

    std::vector<std::string> get_keys() const;
    std::vector<std::string> get_values(const std::string &key) const;

    void clear();
};

}

#endif // DATACONTAINER_H
