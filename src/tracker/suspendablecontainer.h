#ifndef SUSPENDABLECONTAINER_H
#define SUSPENDABLECONTAINER_H

#include "suspendableloader.h"

#include <string>

namespace WT {

class SuspendableContainer
{
    SuspendableLoader loader;

    std::vector<ITrackSuspendable*> suspendable;

public:
    SuspendableContainer(const std::string &plugin_path);

    bool suspend_tracking() const;
};

}

#endif // SUSPENDABLECONTAINER_H
