#include "suspendableloader.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

namespace WT {

SuspendableLoader::SuspendableLoader(const std::string &plugin_dir_path)
{
    load_handlers(plugin_dir_path);
}

SuspendableLoader::~SuspendableLoader()
{
    suspendable.clear();
    handlers.clear();
}

void SuspendableLoader::load_handlers(const std::string &plugin_dir_path)
{
    namespace fs = boost::filesystem;
    fs::path dir_path = plugin_dir_path;

    if (!fs::exists(dir_path) || !fs::is_directory(dir_path))
    {
        return;
    }

    fs::directory_iterator it(dir_path);
    fs::directory_iterator endit;

    while (it != endit)
    {
        auto path = it->path();
        ++it;

        if (fs::is_regular_file(path) && path.extension() == ".so")
        {
            // TODO error handling
            auto handle = std::make_shared<boost::dll::shared_library>(path, boost::dll::load_mode::append_decorations);
            if (!handle)
            {
                WT_LOG_W << "cannot open dynamic library " << it->path()<< ": "<< dlerror();
                continue;
            }

            WT_LOG_D << "library " << path << " has been loaded";

            typedef WT::ITrackSuspendable* (suspendable_create_t)();
            auto create = handle->get<suspendable_create_t>("create_suspendable");
            if (!create)
            {
                WT_LOG_W << "cannot load symbol 'create_suspendable'";
            }
            else
            {
                WT_LOG_D << "symbol 'create_suspendable' has been found";
                handlers.push_back(handle);
                suspendable.push_back(std::shared_ptr<ITrackSuspendable>(create()));
                WT_LOG_D << "Add pluign " << suspendable.back()->get_name();
            }
        }
    }
}

std::vector<std::shared_ptr<ITrackSuspendable>> SuspendableLoader::get_suspendable() const
{
    return suspendable;
}

}
