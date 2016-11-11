#include "suspendableloader.h"

#include "wtcommon/logger.h"

#include <dlfcn.h>
#include <boost/filesystem.hpp>

namespace WT {

SuspendableLoader::SuspendableLoader(const std::string &plugin_dir_path)
{
    load_handlers(plugin_dir_path);
}

SuspendableLoader::~SuspendableLoader()
{
    suspendable.clear();

    for (auto handle : handlers)
    {
        if (dlclose(handle))
        {
            WT_LOG_ERR << "Error on closing handler: " << dlerror();
        }
    }
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
            void* handle = dlopen(path.c_str(), RTLD_NOW);
            if (!handle)
            {
                WT_LOG_W << "cannot open dynamic library " << it->path()<< ": "<< dlerror();
                continue;
            }

            WT_LOG_D << "library " << path << " has been loaded";

            auto create = (ITrackSuspendable* (*)())dlsym(handle, "create_suspendable");
            if (!create)
            {
                WT_LOG_W << "cannot load symbol 'create_suspendable'";
            }
            else
            {
                WT_LOG_D << "symbol 'create_suspendable' has been found";
                handlers.push_back(handle);
                suspendable.push_back(std::shared_ptr<ITrackSuspendable>(create()));
            }
        }
    }
}

std::vector<std::shared_ptr<ITrackSuspendable>> SuspendableLoader::get_suspendable() const
{
    return suspendable;
}

}
