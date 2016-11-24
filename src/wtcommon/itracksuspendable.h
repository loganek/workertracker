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

#include "wtcommon/registrable.h"

namespace WT {

class ITrackSuspendable : public RegistrableCollection<ITrackSuspendable>
{
public:
    virtual ~ITrackSuspendable() {}

    virtual bool suspend_tracking(const char *app_name, const char *window_title) = 0;

    virtual void load_configuration(const char **config[2], int size) = 0;

    virtual const char* get_name() = 0;
};

}

#ifdef WIN32
# define WT_PLUGIN_EXPORT __declspec(dllexport)
#else
# define WT_PLUGIN_EXPORT // empty
#endif

#endif // ITRACKSUSPENDABLE_H
