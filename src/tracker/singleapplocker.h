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

namespace WT {

class SingleAppLocker : public RegistrableSingle<SingleAppLocker>
{
public:
    virtual ~SingleAppLocker() {}

    virtual bool lock() = 0;
    virtual void unlock() = 0;
};

}

#endif // SINGLEAPPLOCKER_H
