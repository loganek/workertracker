/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef SINGLEAPPLOCKER_H
#define SINGLEAPPLOCKER_H

#include "wtcommon/registrable.h"
#include "wtcommon/logger.h"

namespace WT {

class SingleAppLocker : public RegistrableSingle<SingleAppLocker>
{
public:
    virtual ~SingleAppLocker() {}

    virtual bool lock() = 0;
    virtual void unlock() = 0;

    static bool lock_app_instance()
    {
        if (auto app_locker = SingleAppLocker::registry())
        {
            if (!app_locker->lock())
            {
                WT_LOG_EMG << "Cannot lock application. Probably one instance of this program is already running on the system!";
                return false;
            }
        }
        else
        {
            WT_LOG_W << "SingleAppLocker not registred in the system!";
        }
        return true;
    }
};

}

#endif // SINGLEAPPLOCKER_H
