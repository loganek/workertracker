#ifndef BACKGROUNDRUNNER_H
#define BACKGROUNDRUNNER_H

#include "wtcommon/registrable.h"

namespace WT {

class BackgroundRunner : public RegistrableSingle<BackgroundRunner>
{
public:
    virtual ~BackgroundRunner() {}

    virtual int move_to_background() = 0;
    virtual int kill_process() = 0;
    virtual void register_kill_method(void(*handler)(int)) = 0;
};

}

#endif // BACKGROUNDRUNNER_H
