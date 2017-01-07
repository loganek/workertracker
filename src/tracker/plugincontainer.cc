/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "plugincontainer.h"

#include "wtcommon/logger.h"

namespace WT {

PluginContainer::PluginContainer(const std::shared_ptr<PluginsConfiguration> &configuration)
{
    plugins = PluginLoader(configuration->get_plugins_paths()).get_plugins();

    for (const auto &plugin : plugins)
    {
        load_configuration_to_plugin(plugin, configuration);
    }

    std::sort(plugins.begin(), plugins.end(), [] (const std::shared_ptr<PluginWrapper>& left, const std::shared_ptr<PluginWrapper>& right) {
        return left->get_rank() > right->get_rank();
    });
}

void PluginContainer::load_configuration_to_plugin(const std::shared_ptr<PluginWrapper>& plugin, const std::shared_ptr<PluginsConfiguration> &configuration)
{
    auto plugin_config = configuration->get_plugin_configuration(plugin->get_name());

    plugin->load_configuration((const char***)plugin_config.first, plugin_config.second);
    configuration->free_configuration(plugin_config);
}

bool PluginContainer::process_controllers(DataEntry &entry)
{
    WT_LOG_D << "Updating data...";

    char app_name[WT_MAX_APP_NAME_LEN] = {0};
    char window_title[WT_MAX_WIN_TITLE_LEN] = {0};

    strncpy(app_name, entry.proc_name.c_str(), WT_MAX_APP_NAME_LEN-1);
    strncpy(window_title, entry.description.c_str(), WT_MAX_WIN_TITLE_LEN-1);

    for (const auto &plugin : plugins)
    {
        if (plugin->suspend_logging(app_name, window_title))
        {
            return true;
        }
    }

    for (const auto &plugin : plugins)
    {
        if (plugin->update_data_entry(app_name, window_title))
        {
            WT_LOG_D << "Force break from plugin " << plugin->get_name();
            break;
        }
    }

    if (strcmp(app_name, entry.proc_name.c_str()))
    {
        entry.proc_name = app_name;
    }

    if (strcmp(window_title, entry.description.c_str()))
    {
        entry.description = window_title;
    }

    return false;
}


}
