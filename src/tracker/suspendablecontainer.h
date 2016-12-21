/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef SUSPENDABLECONTAINER_H
#define SUSPENDABLECONTAINER_H

#include "plugincontainer.h"
#include "windowinfoprovider.h"

#include "wtcommon/itracksuspendable.h"

#include <functional>
#include <string>

namespace WT {

class SuspendableContainer : public PluginContainer<ITrackSuspendable>
{
public:
    SuspendableContainer(const std::shared_ptr<PluginsConfiguration> &configuration);

    bool suspend_tracking(const WindowInfoProvider::Info &window_info) const;
};

}

#endif // SUSPENDABLECONTAINER_H
