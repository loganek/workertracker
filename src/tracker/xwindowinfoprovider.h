#ifndef XWINDOWINFOPROVIDER_H
#define XWINDOWINFOPROVIDER_H

#include "windowinfoprovider.h"

#include <X11/Xlib.h>

#include <string>

namespace WT {

class XWindowInfoProvider : public WindowInfoProvider
{
    Display *display;
    Info window_info;

    std::string get_window_title(Window win);
    std::string get_app_name(Window win);

    static RegistrarSingle<XWindowInfoProvider> registrar;

public:
    XWindowInfoProvider(const char* display_name = nullptr);

    Info get_current_window_info() override;
};

}

#endif // XWINDOWINFOPROVIDER_H
