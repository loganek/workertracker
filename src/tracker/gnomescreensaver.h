#ifndef GNOMESCREENSAVER_H
#define GNOMESCREENSAVER_H

#include "wtcommon/itracksuspendable.h"

struct _GDBusConnection;
struct _GDBusMessage;

namespace WT {

class GNOMEScreenSaver : public ITrackSuspendable
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

    bool suspend_tracking() override;
    void load_configuration(const char **/*config*/[2], int /*size*/) override {}

    const char* get_name() override { return "gnomescreensaver"; }

    static RegistrarCollection<GNOMEScreenSaver> registrar;
};

}

#endif // GNOMESCREENSAVER_H
