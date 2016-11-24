/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef POSIXSINGLEAPPLOCKER_H
#define POSIXSINGLEAPPLOCKER_H

#include "singleapplocker.h"

#include <string>

namespace WT {

class PosixSingleAppLocker : public SingleAppLocker
{
    static RegistrarSingle<PosixSingleAppLocker> registrar;

    int pid_file_handle = -1;

public:
    virtual ~PosixSingleAppLocker() { unlock(); }
    bool lock() override;
    void unlock() override;

    static std::string get_lock_file();
};

}

#endif // POSIXSINGLEAPPLOCKER_H
