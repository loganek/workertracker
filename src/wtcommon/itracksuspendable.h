/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef ITRACKSUSPENDABLE_H
#define ITRACKSUSPENDABLE_H

#include "registrable.h"
#include "plugindefs.h"

namespace WT {

class ITrackSuspendable : public RegistrableCollection<ITrackSuspendable>
{
public:
    virtual bool WT_APICALL suspend_tracking(const char *app_name, const char *window_title) = 0;

    virtual void WT_APICALL load_configuration(const char **config[2], int size) = 0;

    virtual const char* WT_APICALL get_name() = 0;

    virtual void WT_APICALL destroy() = 0;
};

}

extern "C" WT_PLUGIN_EXPORT WT::ITrackSuspendable* WT_APICALL create_suspendable();

#endif // ITRACKSUSPENDABLE_H
