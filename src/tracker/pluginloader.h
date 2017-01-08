/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "wtcommon/idatacontrolplugin.h"

#include <boost/dll/shared_library.hpp>

#include <vector>
#include <functional>

namespace WT {

class PluginWrapper
{
    WT_PluginInfo plugin_info;
    WT_IDataControlPlugin *plugin;
    std::shared_ptr<boost::dll::shared_library> handler;

public:
    PluginWrapper(const std::shared_ptr<boost::dll::shared_library>& handler)
        : handler(handler)
    {
        plugin_info = handler->get<WT_PluginInfo>("wt_plugin_info");
        plugin = plugin_info.create_func();
    }

    virtual ~PluginWrapper()
    {
        plugin_info.destroy_func(plugin);
    }

    const char* get_name() const
    {
        return plugin_info.name;
    }

    int get_rank() const
    {
        return plugin_info.rank;
    }

    void set_rank(int rank)
    {
        if (rank >= 0)
        {
            plugin_info.rank = rank;
        }
    }

    int get_version() const
    {
        return plugin_info.version;
    }

    bool update_data_entry(char in_out_app_name[WT_MAX_APP_NAME_LEN],
                            char in_out_window_title[WT_MAX_WIN_TITLE_LEN]);

    bool suspend_logging(char in_out_app_name[WT_MAX_APP_NAME_LEN],
                            char in_out_window_title[WT_MAX_WIN_TITLE_LEN]);

    void load_configuration(const char*** config, int size)
    {
        if (plugin_info.load_config_func)
        {
            plugin_info.load_config_func(plugin, config, size);
        }
    }
};

class PluginLoader
{
    std::vector<std::shared_ptr<PluginWrapper>> plugins;

    void try_load_plugin(const std::string &path);
    void load_handlers(const std::string &plugin_dir_path);

public:
    PluginLoader(const std::vector<std::string> &plugin_dir_paths);

    std::vector<std::shared_ptr<PluginWrapper>> get_plugins() const;
};

}

#endif // PLUGINLOADER_H
