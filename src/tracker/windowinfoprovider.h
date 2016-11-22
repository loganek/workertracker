#ifndef WINDOWINFOPROVIDER_H
#define WINDOWINFOPROVIDER_H

#include "wtcommon/registrable.h"

#include <string>
#include <algorithm>

namespace WT {

class WindowInfoProvider : public RegistrableSingle<WindowInfoProvider>
{
    static constexpr char const* unknown_value = "(unknown)";

public:
    struct Info
    {
        static void strip_non_printable(std::string &str)
        {
            str.erase(std::remove_if(str.begin(),str.end(), [](char c) { return !::isprint(c);}), str.end());
        }

        std::string app_name;
        std::string window_title;

        Info(const std::string& app_name, const std::string &window_title)
            : app_name(app_name), window_title(window_title)
        {
            strip_non_printable(this->app_name), strip_non_printable(this->window_title);

        }

        Info() : app_name(unknown_value), window_title(unknown_value) {}
    };

    virtual ~WindowInfoProvider() {}

    virtual Info get_current_window_info() = 0;
};

}

#endif // WINDOWINFOPROVIDER_H
