/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef DAEMONIZER_H
#define DAEMONIZER_H

#include "backgroundrunner.h"

#include <string>

namespace WT {

class Daemonizer : public BackgroundRunner
{
    std::string daemon_name;

    static RegistrarSingle<Daemonizer> registrar;

public:
    Daemonizer(const std::string &daemon_name);

    int move_to_background() override;
    int kill_process() override;
    void register_kill_method(void(*handler)(int)) override;
};

}

#endif // DAEMONIZER_H
