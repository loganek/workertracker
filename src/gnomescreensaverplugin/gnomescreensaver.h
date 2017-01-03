/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef GNOMESCREENSAVER_H
#define GNOMESCREENSAVER_H

struct _GDBusConnection;
struct _GDBusMessage;

class GNOMEScreenSaver
{
    const char* const dbus_service = "org.freedesktop.DBus";
    const char* const dbus_path = "/org/freedesktop/DBus";
    const char* const dbus_interface = "org.freedesktop.DBus";

    const char* const gs_service = "org.gnome.ScreenSaver";
    const char* const gs_path = "/org/gnome/ScreenSaver";
    const char* const gs_interface = "org.gnome.ScreenSaver";

    _GDBusConnection *connection = nullptr;

    bool is_running();
    _GDBusMessage* send_get_active_message();

public:
    virtual ~GNOMEScreenSaver();

    bool suspend_tracking();
};

#endif // GNOMESCREENSAVER_H
