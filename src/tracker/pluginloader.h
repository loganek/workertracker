/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <boost/dll/shared_library.hpp>

#include <vector>
#include <functional>

namespace WT {

template<typename PluginType>
class PluginLoader
{
    std::vector<std::shared_ptr<boost::dll::shared_library>> handlers;
    std::vector<std::shared_ptr<PluginType>> plugins;

    void try_load_plugin(const std::string &path);
    void load_handlers(const std::string &plugin_dir_path);

public:
    PluginLoader(const std::vector<std::string> &plugin_dir_paths);
    virtual ~PluginLoader();

    std::vector<std::shared_ptr<PluginType>> get_plugins() const;
};

}

#endif // PLUGINLOADER_H
