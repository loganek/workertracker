#include "gnomescreensaver.h"

#include <gio/gio.h>

namespace WT {
GNOMEScreenSaver::RegistrarCollection<GNOMEScreenSaver> GNOMEScreenSaver::registrar;

bool GNOMEScreenSaver::is_running()
{
    if (!connection)
    {
        GError *error = nullptr;
        connection = g_bus_get_sync (G_BUS_TYPE_SESSION, nullptr, &error);

        if (connection == nullptr)
        {
            g_error_free (error);
            return false;
        }
    }

    GVariant *reply = g_dbus_connection_call_sync(connection,
                                                  dbus_service,
                                                  dbus_path,
                                                  dbus_interface,
                                                  "GetNameOwner",
                                                  g_variant_new ("(s)", gs_service),
                                                  nullptr,
                                                  G_DBUS_CALL_FLAGS_NO_AUTO_START,
                                                  100,
                                                  nullptr,
                                                  nullptr);

    //if (reply)
    {
        g_variant_unref(reply);
    }

    return reply != nullptr;
}

GDBusMessage *GNOMEScreenSaver::send_get_active_message()
{
    GDBusMessage *message = g_dbus_message_new_method_call(gs_service,
                                                           gs_path,
                                                           gs_interface,
                                                           "GetActive");
    if (message == nullptr)
    {
        return nullptr;
    }

    GError *error = nullptr;
    GDBusMessage *reply = g_dbus_connection_send_message_with_reply_sync(connection,
                                                                         message,
                                                                         G_DBUS_SEND_MESSAGE_FLAGS_NONE,
                                                                         -1,
                                                                         NULL,
                                                                         NULL,
                                                                         &error);


    if (error != nullptr)
    {
        g_clear_error (&error);
    }

    g_dbus_connection_flush_sync(connection, NULL, &error);

    if (error != nullptr)
    {
        g_clear_error (&error);
    }

    g_object_unref(message);

    return reply;
}


bool GNOMEScreenSaver::suspend_tracking()
{
    if (!is_running())
    {
        return false;
    }

    GDBusMessage *reply = send_get_active_message();
    if (reply == nullptr)
    {
        return false;
    }

    GVariant *body = g_dbus_message_get_body(reply);
    gboolean v;
    g_variant_get (body, "(b)", &v);

    g_object_unref (reply);

    return v;
}

GNOMEScreenSaver::~GNOMEScreenSaver()
{
    if (connection)
    {
        g_object_unref(connection);
    }
}

}
