/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef X11WINDOWINFOPROVIDER_H
#define X11WINDOWINFOPROVIDER_H

#include "windowinfoprovider.h"

#include <X11/Xlib.h>

#include <string>

namespace WT {

class X11WindowInfoProvider : public WindowInfoProvider
{
    Display *display = nullptr;
    int screen;

    std::string read_app_from_pid(Window win);
    std::string get_window_title(Window win);
    std::string get_app_name(Window win);
    std::string get_string_property(Window win, const char *property_name);
    unsigned char* get_window_property(Window win, const char *property_name);

    static RegistrarSingle<X11WindowInfoProvider> registrar;

public:
    ~X11WindowInfoProvider() override;

    bool initialize(const std::shared_ptr<WIProviderConfiguration> &configuration) override;
    Info get_current_window_info() override;
};

}

#endif // X11WINDOWINFOPROVIDER_H
