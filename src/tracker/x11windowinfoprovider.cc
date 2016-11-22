#include "x11windowinfoprovider.h"

#include <X11/Xutil.h>

namespace WT {

X11WindowInfoProvider::RegistrarSingle<X11WindowInfoProvider> X11WindowInfoProvider::registrar;

X11WindowInfoProvider::X11WindowInfoProvider(const char *display_name)
{
    display = XOpenDisplay(display_name);
}

X11WindowInfoProvider::~X11WindowInfoProvider()
{
    XCloseDisplay(display);
}

std::string X11WindowInfoProvider::get_window_title(Window win)
{
    std::string ret;

    Window root = -1, *children = nullptr;
    unsigned int children_cnt;
    int status;

    do {
        char *name;
        status = XFetchName(display, win, &name);
        if (status)
        {
            ret = name;
            XFree(name);
            if (!ret.empty()) break;
        }

        XTextProperty text;
        status = XGetWMName(display, win, &text);

        if (status && text.nitems > 0)
        {
            ret = reinterpret_cast<char*>(text.value);
            XFree(text.value);
        }

        XFree(children);
    } while (ret.empty() && win != root && (status = XQueryTree(display, win, &root, &win, &children, &children_cnt)));

    XFree(children);

    return ret;
}

std::string X11WindowInfoProvider::get_app_name(Window win)
{
    int status;
    XClassHint clh;
    Window root = -1;

    while (!(status = XGetClassHint(display, win, &clh)) && root != win)
    {
        Window *children;
        unsigned int children_cnt;

        status = XQueryTree(display, win, &root, &win, &children, &children_cnt);
        XFree(children);

        if (!status)
        {
            break;
        }
    }

    std::string ret;

    if (!status)
    {
        return ret;
    }

    ret = clh.res_name;
    if (ret.empty())
    {
        ret = clh.res_class;
    }

    XFree(clh.res_name);
    XFree(clh.res_class);

    return ret;
}

X11WindowInfoProvider::Info X11WindowInfoProvider::get_current_window_info()
{
    Window window;
    int rev;

    XGetInputFocus(display, &window, &rev);

    return window ? Info(get_window_title(window), get_app_name(window)) : Info();
}

}
