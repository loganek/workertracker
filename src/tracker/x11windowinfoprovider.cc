/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "x11windowinfoprovider.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

namespace WT {

X11WindowInfoProvider::RegistrarSingle<X11WindowInfoProvider> X11WindowInfoProvider::registrar;

bool X11WindowInfoProvider::initialize(const std::shared_ptr<WIProviderConfiguration> &configuration)
{
    auto x11_config = std::dynamic_pointer_cast<X11Configuration>(configuration);
    auto display_name_str = x11_config ? x11_config->get_x11_display_name() : "";
    const char *display_name = display_name_str.c_str();
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

std::string X11WindowInfoProvider::read_app_from_pid(Window win)
{
    auto pid_p = get_window_property(win, "_NET_WM_PID");
    unsigned long pid = pid_p[0] + (pid_p[1]<<8) + (pid_p[2]<<16) + (pid_p[3]<<24);

    if (!pid) return "";

    boost::filesystem::path cmd("/proc");
    cmd /= std::to_string(pid);
    cmd /= "cmdline";

    std::string proc;
    if (boost::filesystem::exists(cmd))
    {
        std::ifstream file(cmd.string(), std::ios::binary);
        std::streambuf* raw_buffer = file.rdbuf();
        char c;
        do {
            c = raw_buffer->sgetc();
                proc += c;
        } while (c != 0 && raw_buffer->snextc() != EOF);
    }

    return boost::filesystem::path(proc).filename().string();
}

std::string X11WindowInfoProvider::get_app_name(Window win)
{
    std::string app_name = get_string_property(win, "WM_CLASS");

    if (app_name.empty())
    {
        app_name = read_app_from_pid(win);
    }

    return app_name;
}

X11WindowInfoProvider::Info X11WindowInfoProvider::get_current_window_info()
{
    Window window = XRootWindow(display, screen);
    unsigned char* net_active_wnd = get_window_property(window, "_NET_ACTIVE_WINDOW");
    window = net_active_wnd[0] + (net_active_wnd[1] << 8) + (net_active_wnd[2] << 16) + (net_active_wnd[3] << 24);

    return window ? Info(get_app_name(window), get_window_title(window)) : Info();
}

}
