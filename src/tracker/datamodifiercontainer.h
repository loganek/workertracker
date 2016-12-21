#ifndef DATAMODIFIERCONTAINER_H
#define DATAMODIFIERCONTAINER_H

#include "plugincontainer.h"

#include "wtcommon/dataentry.h"
#include "wtcommon/idatamodifier.h"

namespace WT
{

class DataModifierContainer : public PluginContainer<IDataModifier>
{
    std::vector<std::shared_ptr<IDataModifier>> sorted_plugins;

public:
    DataModifierContainer(const std::shared_ptr<PluginsConfiguration> &configuration);

    void update_data(DataEntry &window_info) const;
};

}

#endif // DATAMODIFIERCONTAINER_H
