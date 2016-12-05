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

#include <vector>

namespace WT {

class GeneralConfiguration
{
public:
    virtual ~GeneralConfiguration() {}

    virtual int get_read_info_interval() = 0;
};

class PluginsConfiguration
{
public:
    virtual ~PluginsConfiguration() {}

    virtual std::vector<std::string> get_plugins_paths() = 0;
    virtual void add_plugins_path(const std::string &plugins_path) = 0;
    virtual std::pair<char***, int> get_plugin_configuration(const std::string &plugin_name) = 0;
    virtual void free_configuration(const std::pair<char***, int> &config) = 0;
};

class WIProviderConfiguration
{
public:
    virtual ~WIProviderConfiguration() {}
};

class SQLiteDAConfiguration
{
public:
    virtual ~SQLiteDAConfiguration() {}
    virtual int get_persist_frequency() = 0;
};

class X11Configuration : public WIProviderConfiguration
{
public:
    virtual ~X11Configuration() {}

    virtual std::string get_x11_display_name() = 0;
};

class Configuration :
        public GeneralConfiguration,
        public X11Configuration,
        public SQLiteDAConfiguration,
        public PluginsConfiguration
{
    boost::property_tree::ptree prop_tree;
    std::string path;

public:
    Configuration(const std::string &path = WT::Configuration::get_default_config_path());

    static std::string get_default_config_path();

    // PluginsConfiguration
    std::pair<char***, int> get_plugin_configuration(const std::string &plugin_name) override;
    void free_configuration(const std::pair<char***, int> &config) override;
    void add_plugins_path(const std::string &plugins_path) override;
    std::vector<std::string> get_plugins_paths() override;

    // GeneralConfiguration
    virtual int get_read_info_interval() override;

    // X11Configuration
    std::string get_x11_display_name() override;

    // SQLiteDAConfiguration
    int get_persist_frequency() override;
};

}

#endif // CONFIGURATION_H
