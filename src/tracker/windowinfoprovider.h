#ifndef WINDOWINFOPROVIDER_H
#define WINDOWINFOPROVIDER_H

#include "wtcommon/registrable.h"
#include "wtcommon/configuration.h"

#include <string>
#include <algorithm>

namespace WT {

class WindowInfoProvider : public RegistrableSingle<WindowInfoProvider>
{
    static constexpr char const* unknown_value = "(unknown)";

public:
    struct Info
    {
        std::string app_name;
        std::string window_title;

        Info(const std::string& app_name, const std::string &window_title)
            : app_name(app_name), window_title(window_title)
        {
        }

        Info() : app_name(unknown_value), window_title(unknown_value) {}
    };

    virtual ~WindowInfoProvider() {}

    virtual bool initialize(const std::shared_ptr<Configuration> &configuration) = 0;
    virtual Info get_current_window_info() = 0;
};

}

#endif // WINDOWINFOPROVIDER_H
