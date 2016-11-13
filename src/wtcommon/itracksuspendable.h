#ifndef ITRACKSUSPENDABLE_H
#define ITRACKSUSPENDABLE_H

#include "wtcommon/registrable.h"

namespace WT {

class ITrackSuspendable : public RegistrableCollection<ITrackSuspendable>
{
public:
    virtual ~ITrackSuspendable() {}

    virtual bool suspend_tracking() = 0;

    virtual void load_configuration(const char **config[2], int size) = 0;

    virtual const char* get_name() = 0;
};

}

#ifdef WIN32
# define WT_PLUGIN_EXPORT __declspec(dllexport)
#else
# define WT_PLUGIN_EXPORT // empty
#endif

#endif // ITRACKSUSPENDABLE_H
