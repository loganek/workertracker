/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <boost/property_tree/ptree.hpp>

namespace WT {

class Configuration
{
    boost::property_tree::ptree prop_tree;
    std::string path;

    const std::string general_name = "general";
    const static std::string project_name;

    void init_default();
    boost::optional<std::string> get_config_node(const std::string &category, const std::string &parameter) const;

public:
    Configuration(const std::string &path = WT::Configuration::get_default_config_path());

    void save_configuration();

    std::pair<char***, int> get_plugin_configuration(const std::string &plugin_name) const;
    static void free_configuration(const std::pair<char***, int> &config);

    boost::optional<std::string> get_general_param(const std::string &param) const;

    template<typename T>
    boost::optional<T> get_general_param(const std::string &param) const;

    void set_general_param(const std::string &param, const std::string &value);

    template<typename T>
    void set_general_param(const std::string &param, const T& value)
    {
        set_general_param(param, std::to_string(value));
    }

    static std::string get_default_config_path();
};

}

#endif // CONFIGURATION_H
