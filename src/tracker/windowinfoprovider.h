/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef WINDOWINFOPROVIDER_H
#define WINDOWINFOPROVIDER_H

#include "wtcommon/registrable.h"
#include "wtcommon/configuration.h"

#include <string>
#include <algorithm>

namespace WT {

class WindowInfoProvider : public RegistrableSingle<WindowInfoProvider>
{
public:
    class Info
    {
        const std::string unknown_value = "(unknown)";

        std::string app_name;
        std::string window_title;

        const std::string & const get_value(const std::string &value) const { return value.empty() ? unknown_value : value; }

     public:
        Info(const std::string& app_name, const std::string &window_title)
            : app_name(app_name), window_title(window_title)
        {
        }

        const std::string & const get_app_name() const { return get_value(app_name); }
        const std::string & const get_window_title() const { return get_value(window_title); }
    };

    virtual ~WindowInfoProvider() {}

    virtual bool initialize(const std::shared_ptr<Configuration> &configuration) = 0;
    virtual Info get_current_window_info() = 0;
};

}

#endif // WINDOWINFOPROVIDER_H
