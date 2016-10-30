#include "xwindowinfoprovider.h"

#include <X11/Xutil.h>

#include <algorithm>

namespace WT {

XWindowInfoProvider::RegistrarSingle<XWindowInfoProvider> XWindowInfoProvider::registrar;

XWindowInfoProvider::XWindowInfoProvider(const char *display_name)
{
    display = XOpenDisplay(display_name);
}

static void strip_non_printable(std::string &str)
{
    str.erase(remove_if(str.begin(),str.end(), [](char c) { return !::isprint(c);}), str.end());
}

std::string XWindowInfoProvider::get_window_title(Window win) const
{
    Window root = -1;
    Window *children = nullptr;
    unsigned int children_cnt;
    int status;
    std::string ret;

    do {
        char *name;
        status = XFetchName(display, win, &name);
        if (status) {
            ret = name;
            XFree(name);
            if (!ret.empty()) break;
        }
        XTextProperty text;
        status = XGetWMName(display, win, &text);
        if (status && text.nitems > 0) {
            ret = reinterpret_cast<char*>(text.value);
            XFree(text.value);
        }

        XFree(children);
    } while (ret.empty() && (status = XQueryTree(display, win, &root, &win, &children, &children_cnt)) && win != root);

    XFree(children);

    strip_non_printable(ret);

    return ret.empty() ? std::string("(unknown)") : ret;
}

std::string XWindowInfoProvider::get_app_name(Window win) const
{
    int status;
    XClassHint clh;
    Window root = -1;
    Window *children;
    unsigned int children_cnt;

    while (!(status = XGetClassHint(display, win, &clh)) && root != win) {
        status = XQueryTree(display, win, &root, &win, &children, &children_cnt);
        XFree(children);
        if (!status) {
            break;
        }
    }

    std::string ret;

    if (status) {
        ret = clh.res_name;
        if (ret.empty()) {
            ret = clh.res_class;
        }
        XFree(clh.res_name);
        XFree(clh.res_class);
    }

    if (ret.empty())
    {
        ret = "(unknown)";
    }

    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    strip_non_printable(ret);

    return ret;
}


XWindowInfoProvider::Info XWindowInfoProvider::get_current_window_info() const
{
    Window window;
    int rev;

    XGetInputFocus(display, &window, &rev);

    return Info {get_window_title(window), get_app_name(window)};
}

}
