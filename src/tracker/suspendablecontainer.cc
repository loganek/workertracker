#include "suspendablecontainer.h"

#include "wtcommon/logger.h"

namespace WT {

SuspendableContainer::SuspendableContainer(const std::string &plugin_path)
    : loader(plugin_path)
{
}

bool SuspendableContainer::suspend_tracking() const
{
    WT_LOG_D << "Checking if tracking should be suspended...";

    for (auto susp : loader.get_suspendable())
    {
        if (susp->suspend_tracking()) return true;
    }

    for (auto susp : RegistrableCollection<ITrackSuspendable>::registry())
    {
        if (susp.second->suspend_tracking()) return true;
    }

    WT_LOG_D << "Tracking NOT suspended";

    return false;
}

}
