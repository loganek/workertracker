/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef BACKGROUNDRUNNER_H
#define BACKGROUNDRUNNER_H

#include "wtcommon/registrable.h"

#include <functional>

namespace WT {

class BackgroundRunner : public RegistrableSingle<BackgroundRunner>
{
public:
    virtual ~BackgroundRunner() {}

    virtual int run_in_background(std::function<int()> callback) = 0;
    virtual int kill_process() = 0;
    virtual void register_kill_method(void(*handler)(int)) = 0;
};

}

#endif // BACKGROUNDRUNNER_H
