#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <boost/property_tree/ptree.hpp>

namespace WT {

class Configuration
{
    boost::property_tree::ptree prop_tree;
    std::string path;

    const std::string general_name = "general";

    void init_default();
    boost::optional<std::string> get_config_node(const std::string &category, const std::string &parameter) const;

public:
    Configuration(const std::string &path);

    void save_configuration();

    std::pair<char***, int> get_plugin_configuration(const std::string &plugin_name) const;

    boost::optional<std::string> get_general_param(const std::string &param) const;

    template<typename T>
    boost::optional<T> get_general_param(const std::string &param) const;

    void set_general_param(const std::string &param, const std::string &value);

    template<typename T>
    void set_general_param(const std::string &param, const T& value)
    {
        set_general_param(param, std::to_string(value));
    }

    void print_all_general_params() const;
};

}

#endif // CONFIGURATION_H
