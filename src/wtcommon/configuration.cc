/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "configuration.h"

#include "logger.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <map>

namespace WT {

const std::string Configuration::project_name = "workertracker";

Configuration::Configuration(const std::string &path)
    : path(path)
{
    if (boost::filesystem::exists(path))
    {
        boost::property_tree::json_parser::read_json(path, prop_tree);
        WT_LOG_I  << "Loaded configuration from " << path;
    }
    else
    {
        WT_LOG_I << "Configuration file " << path << " doesn't exist";
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
        {"plugins-path", (boost::filesystem::path(path).parent_path() / "plugins").string()},
        {"x11-display-name", ""} // actually, it's not general. I need more advanced property tree. In the future.
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
    boost::optional<decltype(prop_tree)&> child_node = prop_tree.get_child(general_name).get_child_optional(param);
    if (child_node)
    {
        child_node.get().data() = value;
    }
    else
    {
        prop_tree.get_child(general_name).push_back(boost::property_tree::ptree::value_type(std::make_pair(param, value)));
    }
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

std::string Configuration::get_default_config_path()
{
    static boost::filesystem::path config_path;

    if (config_path.empty())
    {
#if defined(BOOST_POSIX_API)
        config_path = std::getenv("HOME");
#elif defined(BOOST_WINDOWS_API)
        config_path = boost::filesystem::path(std::getenv("HOMEDRIVE")) / std::getenv("HOMEPATH");
#endif

        config_path /= std::string(".") + project_name;
        config_path /= project_name + ".config";
    }

    return config_path.string();
}

}
