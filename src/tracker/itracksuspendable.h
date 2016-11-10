#ifndef ITRACKSUSPENDABLE_H
#define ITRACKSUSPENDABLE_H

#include "wtcommon/registrable.h"

namespace WT {

class ITrackSuspendable : public RegistrableCollection<ITrackSuspendable>
{
public:
    virtual ~ITrackSuspendable() {}

    virtual bool suspend_tracking() = 0;
};

}

#endif // ITRACKSUSPENDABLE_H
