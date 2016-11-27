/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "suspendablecontainer.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

namespace WT {

SuspendableContainer::SuspendableContainer(const std::shared_ptr<Configuration> &configuration)
    : loader({configuration->get_general_param("plugins-path").get(),
              (boost::filesystem::system_complete(configuration->get_general_param("current-program-path").get()).parent_path() / "plugins").string()})
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
        return susp->suspend_tracking(window_info.get_app_name().c_str(), window_info.get_window_title().c_str());
    }))
    {
        return true;
    }

    WT_LOG_D << "Tracking NOT suspended";
    return false;
}

}
