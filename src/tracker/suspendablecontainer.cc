#include "suspendablecontainer.h"

#include "wtcommon/logger.h"

namespace WT {

SuspendableContainer::SuspendableContainer(const std::shared_ptr<Configuration> &configuration)
    : loader(configuration->get_general_param("plugins-path").get())
{
    load_configuration_to_plugins(configuration);
}

void SuspendableContainer::load_configuration_to_plugins(const std::shared_ptr<Configuration> &configuration)
{
    for (auto susp : loader.get_suspendable())
    {
        auto config = configuration->get_plugin_configuration(susp->get_name());

        if (!config.first)
        {
            continue;
        }

        susp->load_configuration((const char***)config.first, config.second);

        for (int i = 0; i < config.second; i++)
        {
            delete [] config.first[0][i];
            delete [] config.first[1][i];
        }

        delete [] config.first[0];
        delete [] config.first[1];

        delete [] config.first;
    }
}

bool SuspendableContainer::suspend_tracking(const WindowInfoProvider::Info &window_info) const
{
    WT_LOG_D << "Checking if tracking should be suspended...";

    for (auto susp : loader.get_suspendable())
    {
        if (susp->suspend_tracking(window_info.app_name.c_str(), window_info.window_title.c_str()))
        {
            return true;
        }
    }

    for (auto susp : RegistrableCollection<ITrackSuspendable>::registry())
    {
        if (susp.second->suspend_tracking(window_info.app_name.c_str(), window_info.window_title.c_str()))
        {
            return true;
        }
    }

    WT_LOG_D << "Tracking NOT suspended";

    return false;
}

}
