/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "pluginloader.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

namespace WT {

PluginLoader::PluginLoader(const std::vector<std::string> &plugin_dir_paths)
{
    for (const auto &path : plugin_dir_paths)
    {
        WT_LOG_D << "Loading plugins from " << path;
        load_handlers(path);
    }
}

void PluginLoader::load_handlers(const std::string &plugin_dir_path)
{
    namespace fs = boost::filesystem;
    if (!fs::exists(plugin_dir_path) || !fs::is_directory(plugin_dir_path))
    {
        return;
    }

    fs::directory_iterator it(plugin_dir_path);
    fs::directory_iterator endit;

    while (it != endit)
    {
        try_load_plugin(it->path().string());
        ++it;
    }
}

std::vector<std::shared_ptr<PluginWrapper>> PluginLoader::get_plugins() const
{
    return plugins;
}

void PluginLoader::try_load_plugin(const std::string &path)
{
    boost::filesystem::path pt = path;
    if (!boost::filesystem::is_regular_file(pt) || pt.extension() != boost::dll::shared_library::suffix())
    {
        return;
    }

    try
    {
        WT_LOG_D << "Trying to load plugin: " << pt;
        auto handler = std::make_shared<boost::dll::shared_library>(pt);

        plugins.push_back(std::make_shared<PluginWrapper>(handler));

        WT_LOG_I << "Added pluign " << plugins.back()->get_name();
    }
    catch (const boost::system::system_error& err)
    {
        WT_LOG_W << "cannot load plugin " << pt << ": "<< err.what();
    }
}

bool PluginWrapper::process_data_entry(char in_out_app_name[WT_MAX_APP_NAME_LEN],
                                       char in_out_window_title[WT_MAX_WIN_TITLE_LEN],
                                       int* out_force_break)
{
    return plugin_info.control_data_func(plugin, in_out_app_name, in_out_window_title, out_force_break);
}

}
