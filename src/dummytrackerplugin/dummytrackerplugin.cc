#include "wtcommon/itracksuspendable.h"

#include "wtcommon/logger.h"

class DummyTrackerPlugin : public WT::ITrackSuspendable
{
public:
    bool suspend_tracking() override
    {
        WT_STD_LOG(WT::LogLevel::DEBUG) << "I'm not suspending, I'm just dummy plugin";

        return false;
    }
};

extern "C" WT_PLUGIN_EXPORT WT::ITrackSuspendable* create_suspendable()
{
    return new DummyTrackerPlugin();
}
