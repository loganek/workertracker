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

        auto wrapper = std::make_shared<PluginWrapper>(std::make_shared<boost::dll::shared_library>(pt));
        if (wrapper->get_version() != WT_VERSION_MAJOR)
        {
            WT_LOG_D << "Found symbol, but version doesn't match(expected " << WT_VERSION_MAJOR << " but is " << wrapper->get_version() << ")";
            return;
        }

        if (plugins.end() != std::find_if(plugins.begin(), plugins.end(),
                                          [wrapper] (const std::shared_ptr<PluginWrapper>& plugin_wrapper) {
                                          return strcmp(wrapper->get_name(), plugin_wrapper->get_name()) == 0;
    }))
        {
            WT_LOG_W << "Plugin has not been added because plugin with the same name (" << wrapper->get_name() << ") already exists.";
            return;
        }

        plugins.push_back(wrapper);

        WT_LOG_I << "Added pluign " << plugins.back()->get_name();
    }
    catch (const boost::system::system_error& err)
    {
        WT_LOG_W << "cannot load plugin " << pt << ": "<< err.what();
    }
}

bool PluginWrapper::update_data_entry(char in_out_app_name[WT_MAX_APP_NAME_LEN],
                                      char in_out_window_title[WT_MAX_WIN_TITLE_LEN])
{
    if (plugin_info.update_data_func)
    {
        return plugin_info.update_data_func(plugin, in_out_app_name, in_out_window_title);
    }
    return false;
}

bool PluginWrapper::suspend_logging(char in_out_app_name[WT_MAX_APP_NAME_LEN],
                                    char in_out_window_title[WT_MAX_WIN_TITLE_LEN])
{
    if (plugin_info.suspend_tracking_func)
    {
        return plugin_info.suspend_tracking_func(plugin, in_out_app_name, in_out_window_title);
    }
    return false;
}

}
