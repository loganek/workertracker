#ifndef SUSPENDABLELOADER_H
#define SUSPENDABLELOADER_H

#include "wtcommon/itracksuspendable.h"

#include <vector>
#include <functional>

namespace WT {

class SuspendableLoader
{
    std::vector<void*> handlers;
    std::vector<std::shared_ptr<ITrackSuspendable>> suspendable;


    void load_handlers(const std::string &plugin_dir_path);

public:
    SuspendableLoader(const std::string &plugin_dir_path);
    virtual ~SuspendableLoader();

    std::vector<std::shared_ptr<ITrackSuspendable>> get_suspendable() const;
};

}

#endif // SUSPENDABLELOADER_H
