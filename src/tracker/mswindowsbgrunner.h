/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef MSWINDOWSBGRUNNER_H
#define MSWINDOWSBGRUNNER_H

#include "backgroundrunner.h"

#include <string>

namespace WT {

class MSWindowsBGRunner : public BackgroundRunner
{
    std::string log_file_name;

    static RegistrarSingle<MSWindowsBGRunner> registrar;

    std::string MSWindowsBGRunner::get_current_process_name();

public:
    MSWindowsBGRunner(const std::string &log_file_name);

    int move_to_background() override;
    int kill_process() override;
    void register_kill_method(void(*handler)(int)) override;
};

}

#endif // MSWINDOWSBGRUNNER_H
