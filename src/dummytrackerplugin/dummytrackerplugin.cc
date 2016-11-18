#include "wtcommon/itracksuspendable.h"

#include "wtcommon/logger.h"

#include <cstring>

class DummyTrackerPlugin : public WT::ITrackSuspendable
{
    bool silent = false;

public:
    bool suspend_tracking(const char *, const char *) override
    {
        if (!silent)
        {
            WT_LOG_D << "I'm not suspending, I'm just dummy plugin";
        }

        return false;
    }

    void load_configuration(const char **config[2], int size) override
    {
        for (int i = 0; i < size; i++)
        {
            if (strcmp(config[0][i], "silent") == 0)
            {
                silent = std::atoi(config[1][i]);
                WT_LOG_D << "Loaded configuration 'silent': " << silent;
            }
        }
    }

    const char* get_name() override { return "dummytrackerplugin"; }
};

extern "C" WT_PLUGIN_EXPORT WT::ITrackSuspendable* create_suspendable()
{
    return new DummyTrackerPlugin();
}
