#include "datamodifiercontainer.h"

#include "wtcommon/logger.h"

namespace WT
{

DataModifierContainer::DataModifierContainer(const std::shared_ptr<PluginsConfiguration> &configuration)
    : PluginContainer(configuration)
{
    sorted_plugins = loader.get_plugins();
    std::sort(sorted_plugins.begin(), sorted_plugins.end(),
              [](const std::shared_ptr<IDataModifier>& left, const std::shared_ptr<IDataModifier>& right){
        return left->get_rank() > right->get_rank();
    });
}


void DataModifierContainer::update_data(DataEntry &entry) const
{
    WT_LOG_D << "Updating data...";

    bool force_break = false;
    char in_out_app_name[WT_MAX_APP_NAME_LEN] = {0};
    char in_out_window_title[WT_MAX_WIN_TITLE_LEN] = {0};

    strncpy(in_out_app_name, entry.proc_name.c_str(), WT_MAX_APP_NAME_LEN-1);
    strncpy(in_out_window_title, entry.description.c_str(), WT_MAX_WIN_TITLE_LEN-1);

    for (const auto& plugin : sorted_plugins)
    {
        plugin->modify_data(in_out_app_name, in_out_window_title, force_break);

        if (force_break)
        {
            WT_LOG_D << "Force break from plugin " << plugin->get_name();
            break;
        }
    }

    if (strcmp(in_out_app_name, entry.proc_name.c_str()))
    {
        entry.proc_name = in_out_app_name;
    }

    if (strcmp(in_out_window_title, entry.description.c_str()))
    {
        entry.description = in_out_window_title;
    }
}
}
