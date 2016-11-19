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
    SuspendableLoader(const std::string &plugin_dir_path);
    virtual ~SuspendableLoader();

    std::vector<std::shared_ptr<ITrackSuspendable>> get_suspendable() const;
};

}

#endif // SUSPENDABLELOADER_H
