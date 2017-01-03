/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H

#include "pluginloader.h"

#include "wtcommon/configuration.h"
#include "wtcommon/dataentry.h"
namespace WT
{

class PluginContainer
{
    std::vector<std::shared_ptr<PluginWrapper>> plugins;

    void load_configuration_to_plugin(const std::shared_ptr<PluginWrapper>& plugin, const std::shared_ptr<PluginsConfiguration> &configuration);

public:
    PluginContainer(const std::shared_ptr<PluginsConfiguration> &configuration);

    bool process_controllers(DataEntry &data_entry);
};



}

#endif // PLUGINCONTAINER_H
