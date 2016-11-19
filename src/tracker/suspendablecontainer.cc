#include "suspendablecontainer.h"

#include "wtcommon/logger.h"

namespace WT {

SuspendableContainer::SuspendableContainer(const std::shared_ptr<Configuration> &configuration)
    : loader(configuration->get_general_param("plugins-path").get())
{
    load_configuration_to_plugins(configuration);
}

bool SuspendableContainer::foreach_suspendable(std::function<bool(const std::shared_ptr<ITrackSuspendable>&)> func) const
{
    for (auto susp : loader.get_suspendable())
    {
        if (func(susp)) return true;
    }

    for (auto susp : RegistrableCollection<ITrackSuspendable>::registry())
    {
        if (func(susp.second)) return true;
    }

    return false;
}

void SuspendableContainer::load_configuration_to_plugins(const std::shared_ptr<Configuration> &configuration)
{
    foreach_suspendable([&configuration](const std::shared_ptr<ITrackSuspendable> &susp)
    {
        auto plugin_config = configuration->get_plugin_configuration(susp->get_name());

        susp->load_configuration((const char***)plugin_config.first, plugin_config.second);
        configuration->free_configuration(plugin_config);

        return false;
    });
}

bool SuspendableContainer::suspend_tracking(const WindowInfoProvider::Info &window_info) const
{
    WT_LOG_D << "Checking if tracking should be suspended...";

    if (foreach_suspendable([&window_info](const std::shared_ptr<ITrackSuspendable> &susp)
    {
        return susp->suspend_tracking(window_info.app_name.c_str(), window_info.window_title.c_str());
    }))
    {
        return true;
    }

    WT_LOG_D << "Tracking NOT suspended";
    return false;
}

}
