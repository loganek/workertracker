#ifndef DAEMONIZER_H
#define DAEMONIZER_H

#include "backgroundrunner.h"

#include <string>

namespace WT {

class Daemonizer : public BackgroundRunner
{
    int pid_file_handle;
    std::string daemon_name;
    std::string pid_file_name;

    static RegistrarSingle<Daemonizer> registrar;

public:
    Daemonizer(const std::string &daemon_name);
    virtual ~Daemonizer();

    int move_to_background() override;
    int kill_process() override;
    void register_kill_method(void(*handler)(int)) override;
};

}

#endif // DAEMONIZER_H
