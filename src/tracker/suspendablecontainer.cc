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

SuspendableContainer::SuspendableContainer(const std::shared_ptr<PluginsConfiguration> &configuration)
    : PluginContainer(configuration)
{
    for (auto susp : RegistrableCollection<ITrackSuspendable>::registry())
    {
        load_configuration_to_plugin(susp.second, configuration);
    }

}

bool SuspendableContainer::suspend_tracking(const DataEntry& entry) const
{
    WT_LOG_D << "Checking if tracking should be suspended...";

    for (auto susp : RegistrableCollection<ITrackSuspendable>::registry())
    {
        if (susp.second->suspend_tracking(entry.proc_name.c_str(), entry.description.c_str()))
        {
            return true;
        }
    }

    for (auto susp : loader.get_plugins())
    {
        if (susp->suspend_tracking(entry.proc_name.c_str(), entry.description.c_str()))
        {
            return true;
        }
    }

    WT_LOG_D << "Tracking NOT suspended";
    return false;
}

}
