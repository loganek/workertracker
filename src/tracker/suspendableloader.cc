/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "suspendableloader.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

namespace WT {

SuspendableLoader::SuspendableLoader(const std::initializer_list<std::string> &plugin_dir_paths)
{
    for (const auto &path : plugin_dir_paths)
    {
        WT_LOG_D << "Loading plugins from " << path;
        load_handlers(path);
    }
}

SuspendableLoader::~SuspendableLoader()
{
    suspendable.clear();
    handlers.clear();
}

void SuspendableLoader::load_handlers(const std::string &plugin_dir_path)
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

std::vector<std::shared_ptr<ITrackSuspendable>> SuspendableLoader::get_suspendable() const
{
    return suspendable;
}

void SuspendableLoader::try_load_plugin(const std::string &path)
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

        typedef WT::ITrackSuspendable* (suspendable_create_t)();
        auto create = handle->get<suspendable_create_t>("create_suspendable");

        handlers.push_back(handle);
        suspendable.push_back(std::shared_ptr<ITrackSuspendable>(create()));

        WT_LOG_I << "Added pluign " << suspendable.back()->get_name();
    }
    catch (const boost::system::system_error& err)
    {
        WT_LOG_W << "cannot load plugin " << pt << ": "<< err.what();
    }
}

}
