#include "configuration.h"

#include "logger.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <map>

namespace WT {

Configuration::Configuration(const std::string &path)
    : path(path)
{
    // TODO error handling
    if (boost::filesystem::exists(path))
    {
        boost::property_tree::json_parser::read_json(path, prop_tree);
    }

    init_default();
}

void Configuration::init_default()
{
    if (!prop_tree.get_child_optional(general_name))
    {
        prop_tree.add_child(general_name, boost::property_tree::ptree());
    }

    std::map<std::string, std::string> default_config = {
        {"save-period", "12"},
        {"read-period", "5"},
        {"data-path", (boost::filesystem::path(path).parent_path() / "data.dat").string()},
        {"plugins-path", (boost::filesystem::path(path).parent_path() / "plugins").string()}
    };

    auto& general = prop_tree.get_child(general_name);
    for (const auto &entry : default_config)
    {
        if (!general.get_child_optional(entry.first))
        {
            set_general_param(entry.first, entry.second);
        }
    }
}

void Configuration::save_configuration()
{
    try
    {
        boost::property_tree::json_parser::write_json(path, prop_tree);
    }
    catch (const boost::property_tree::json_parser_error &error)
    {
        WT_LOG_ERR << "Cannot save configuration: " << error.what();
    }
}

boost::optional<std::string> Configuration::get_config_node(const std::string &category, const std::string &parameter) const
{
    auto general = prop_tree.get_child_optional(category);
    if (general)
    {
        auto child = general.get().get_child_optional(parameter);

        if (child)
            return child.get().data();
    }

    return boost::none;
}

boost::optional<std::string> Configuration::get_general_param(const std::string &param) const
{
    return get_config_node(general_name, param);
}

void Configuration::set_general_param(const std::string &param, const std::string &value)
{
    prop_tree.get_child(general_name).push_back(boost::property_tree::ptree::value_type(std::make_pair(param, value)));
}

std::pair<char***, int> Configuration::get_plugin_configuration(const std::string &plugin_name) const
{
    char*** config = new char**[2] {nullptr, nullptr};

    auto plugin_cfg = prop_tree.get_child_optional("plugin_" + plugin_name);
    int size = 0;

    if (plugin_cfg && plugin_cfg.get().size() > 0)
    {
        config[0] = new char*[plugin_cfg.get().size()];
        config[1] = new char*[plugin_cfg.get().size()];

        for (auto entry : plugin_cfg.get())
        {
            config[0][size] = new char[entry.first.size() + 1];
            strcpy(config[0][size], entry.first.c_str());
            config[1][size] = new char[entry.second.data().size() + 1];
            strcpy(config[1][size], entry.second.data().c_str());
            size++;
        }
    }

    return std::make_pair(config, size);
}

void Configuration::free_configuration(const std::pair<char***, int> &config)
{
    for (int i = 0; i < config.second; i++)
    {
        delete [] config.first[0][i];
        delete [] config.first[1][i];
    }

    delete [] config.first[0];
    delete [] config.first[1];
}

template<>
boost::optional<int> Configuration::get_general_param(const std::string &param) const
{
    auto v = get_general_param(param);

    return v ? boost::make_optional(std::stoi(v.get())) : boost::none;
}

}
