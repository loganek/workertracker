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
#include <boost/algorithm/string.hpp>

#include <map>

namespace WT {

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

    add_plugins_path((boost::filesystem::path(path).parent_path() / "plugins").string());
}

std::vector<std::string> Configuration::get_plugins_paths()
{
    auto values = prop_tree.get("plugins.plugins-paths", "");
    std::vector<std::string> out;
    boost::algorithm::split(out, values, [](char c) { return c == ';'; });
    return out;
}

void Configuration::add_plugins_path(const std::string &plugins_path)
{
    auto current_paths = prop_tree.get("plugins.plugins-paths", "");
    if (!current_paths.empty())
    {
        current_paths += ';';
    }
    current_paths += plugins_path;
    prop_tree.put("plugins.plugins-paths", current_paths);
}

int Configuration::get_read_info_interval()
{
    return prop_tree.get<int>("general.read-info-interval", 5);
}

std::string Configuration::get_x11_display_name()
{
    return prop_tree.get("x11.display-name", "");
}

int Configuration::get_persist_frequency()
{
    return prop_tree.get<int>("sqliteda.persist-frequency", 12);
}

std::pair<char***, int> Configuration::get_plugin_configuration(const std::string &plugin_name)
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

        std::string project_name(WT_PROJECT_NAME);
        config_path /= "." + project_name;
        config_path /= project_name + ".config";
    }

    return config_path.string();
}

}
