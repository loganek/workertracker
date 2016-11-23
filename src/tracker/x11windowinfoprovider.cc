#include "x11windowinfoprovider.h"

#include "wtcommon/logger.h"

namespace WT {

X11WindowInfoProvider::RegistrarSingle<X11WindowInfoProvider> X11WindowInfoProvider::registrar;

bool X11WindowInfoProvider::initialize(const std::shared_ptr<Configuration> &configuration)
{
    auto maybe_display_name = configuration->get_general_param("x11-display-name");
    const char *display_name = maybe_display_name ? maybe_display_name.get().c_str() : nullptr;
    display = XOpenDisplay(display_name);

    if (display == nullptr)
    {
        WT_LOG_EMG << "Cannot open display :" << XDisplayName(display_name);
        return false;
    }

    screen = XDefaultScreen(display);

    return true;
}

X11WindowInfoProvider::~X11WindowInfoProvider()
{
    if (display != nullptr)
    {
        XCloseDisplay(display);
    }
}

unsigned char* X11WindowInfoProvider::get_window_property(Window win, const char *property_name)
{
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    constexpr long MAX_LEN = 1024;
    unsigned char *property_value = nullptr;

    Atom filter_atom = XInternAtom(display, property_name, True);
    int status = XGetWindowProperty(display, win, filter_atom, 0, MAX_LEN, False, AnyPropertyType,
                    &actual_type, &actual_format, &nitems, &bytes_after, &property_value);

    if (status == BadWindow)
    {
        WT_LOG_ERR << "Window " << win << " doesn't exist";
    }
    else if (status != Success)
    {
        WT_LOG_ERR << "XGetWindowProperty() error: " << status;
    }

    return property_value;
}


std::string X11WindowInfoProvider::get_string_property(Window win, const char *property_name)
{
    char *window_title = reinterpret_cast<char*>(get_window_property(win, property_name));

    std::string ret;
    if (window_title)
    {
        ret = window_title;
        XFree(window_title);
    }

    return ret;
}

std::string X11WindowInfoProvider::get_window_title(Window win)
{
    return get_string_property(win, "_NET_WM_NAME");
}

std::string X11WindowInfoProvider::get_app_name(Window win)
{
    return get_string_property(win, "WM_CLASS");
}

X11WindowInfoProvider::Info X11WindowInfoProvider::get_current_window_info()
{
    Window window = XRootWindow(display, screen);
    unsigned char* net_active_wnd = get_window_property(window, "_NET_ACTIVE_WINDOW");
    window = net_active_wnd[0] + (net_active_wnd[1] << 8) + (net_active_wnd[2] << 16) + (net_active_wnd[3] << 24);

    return window ? Info(get_app_name(window), get_window_title(window)) : Info();
}

}
