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
#include "wtcommon/itracksuspendable.h"
#include "wtcommon/idatamodifier.h"

#include <boost/filesystem.hpp>

namespace WT {

template<typename T>
static const char* get_create_method_name();

template<>
const char* get_create_method_name<WT::ITrackSuspendable>()
{
    return "create_suspendable";
}

template<>
const char* get_create_method_name<WT::IDataModifier>()
{
    return "create_data_modifier";
}

template<typename T>
PluginLoader<T>::PluginLoader(const std::vector<std::string> &plugin_dir_paths)
{
    for (const auto &path : plugin_dir_paths)
    {
        WT_LOG_D << "Loading plugins from " << path;
        load_handlers(path);
    }
}

template<typename T>
PluginLoader<T>::~PluginLoader()
{
    plugins.clear();
    handlers.clear();
}

template<typename T>
void PluginLoader<T>::load_handlers(const std::string &plugin_dir_path)
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

template<typename T>
std::vector<std::shared_ptr<T>> PluginLoader<T>::get_plugins() const
{
    return plugins;
}

template<typename T>
void PluginLoader<T>::try_load_plugin(const std::string &path)
{
    boost::filesystem::path pt = path;
    if (!boost::filesystem::is_regular_file(pt) || pt.extension() != boost::dll::shared_library::suffix())
    {
        return;
    }

    try
    {
        WT_LOG_D << "Trying to load plugin: " << pt;
        auto handle = std::make_shared<boost::dll::shared_library>(pt);

        typedef T* (WT_APICALL plugin_create_t)();
        auto create = handle->get<plugin_create_t>(get_create_method_name<T>());

        handlers.push_back(handle);
        auto obj = create();
        plugins.push_back(std::shared_ptr<T>(obj, std::mem_fn(&T::destroy)));

        WT_LOG_I << "Added pluign " << plugins.back()->get_name();
    }
    catch (const boost::system::system_error& err)
    {
        WT_LOG_W << "cannot load plugin " << pt << ": "<< err.what();
    }
}

template class PluginLoader<WT::ITrackSuspendable>;
template class PluginLoader<WT::IDataModifier>;

}
