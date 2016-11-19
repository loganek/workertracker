#ifndef SUSPENDABLECONTAINER_H
#define SUSPENDABLECONTAINER_H

#include "suspendableloader.h"
#include "windowinfoprovider.h"

#include "wtcommon/configuration.h"

#include <functional>
#include <string>

namespace WT {

class SuspendableContainer
{
    SuspendableLoader loader;

    std::vector<ITrackSuspendable*> suspendable;

    void load_configuration_to_plugins(const std::shared_ptr<Configuration> &configuration);

    bool foreach_suspendable(std::function<bool(const std::shared_ptr<ITrackSuspendable>&)> func) const;

public:
    SuspendableContainer(const std::shared_ptr<Configuration> &configuration);

    bool suspend_tracking(const WindowInfoProvider::Info &window_info) const;
};

}

#endif // SUSPENDABLECONTAINER_H
