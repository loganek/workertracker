/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef SUSPENDABLELOADER_H
#define SUSPENDABLELOADER_H

#include "wtcommon/itracksuspendable.h"

#include <boost/dll/shared_library.hpp>

#include <vector>
#include <functional>

namespace WT {

class SuspendableLoader
{
    std::vector<std::shared_ptr<boost::dll::shared_library>> handlers;
    std::vector<std::shared_ptr<ITrackSuspendable>> suspendable;

    void try_load_plugin(const std::string &path);
    void load_handlers(const std::string &plugin_dir_path);

public:
    SuspendableLoader(const std::initializer_list<std::string> &plugin_dir_paths);
    virtual ~SuspendableLoader();

    std::vector<std::shared_ptr<ITrackSuspendable>> get_suspendable() const;
};

}

#endif // SUSPENDABLELOADER_H
