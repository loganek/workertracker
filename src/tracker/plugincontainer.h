#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H

#include "pluginloader.h"

#include "wtcommon/configuration.h"

namespace WT
{

template <typename PluginType>
class PluginContainer
{
protected:
    PluginLoader<PluginType> loader;
    void load_configuration_to_plugin(const std::shared_ptr<PluginType>& plugin, const std::shared_ptr<PluginsConfiguration> &configuration);

public:
    PluginContainer(const std::shared_ptr<PluginsConfiguration> &configuration)
        : loader(configuration->get_plugins_paths())
    {
        for (const auto &plugin : loader.get_plugins())
        {
            load_configuration_to_plugin(plugin, configuration);
        }
    }

    virtual ~PluginContainer() {}
};

template<typename PluginType>
void PluginContainer<PluginType>::load_configuration_to_plugin(const std::shared_ptr<PluginType>& plugin, const std::shared_ptr<PluginsConfiguration> &configuration)
{
    auto plugin_config = configuration->get_plugin_configuration(plugin->get_name());

    plugin->load_configuration((const char***)plugin_config.first, plugin_config.second);
    configuration->free_configuration(plugin_config);
}

}

#endif // PLUGINCONTAINER_H
