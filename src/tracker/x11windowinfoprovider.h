#ifndef X11WINDOWINFOPROVIDER_H
#define X11WINDOWINFOPROVIDER_H

#include "windowinfoprovider.h"

#include <X11/Xlib.h>

#include <string>

namespace WT {

class X11WindowInfoProvider : public WindowInfoProvider
{
    Display *display;
    Info window_info;

    std::string get_window_title(Window win);
    std::string get_app_name(Window win);

    static RegistrarSingle<X11WindowInfoProvider> registrar;

public:
    X11WindowInfoProvider(const char* display_name = nullptr);
    ~X11WindowInfoProvider() override;

    Info get_current_window_info() override;
};

}

#endif // X11WINDOWINFOPROVIDER_H
