/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef IDATAMODIFIER_H
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#define IDATAMODIFIER_H

#include "plugindefs.h"

namespace WT {

// TODO defined in sqlitedataaccess too..
#define WT_MAX_APP_NAME_LEN 50
#define WT_MAX_WIN_TITLE_LEN 150

class IDataModifier
{
public:
    virtual void WT_APICALL modify_data(
            char in_out_app_name[WT_MAX_APP_NAME_LEN],
            char in_out_window_title[WT_MAX_WIN_TITLE_LEN],
            bool& force_break) = 0;

    virtual int WT_APICALL get_rank() = 0;

    virtual void WT_APICALL load_configuration(const char **config[2], int size) = 0;

    virtual const char* WT_APICALL get_name() = 0;

    virtual void WT_APICALL destroy() = 0;
};

}

extern "C" WT_PLUGIN_EXPORT WT::IDataModifier* WT_APICALL create_data_modifier();

#endif // IDATAMODIFIER_H
