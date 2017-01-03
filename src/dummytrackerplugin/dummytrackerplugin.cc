/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "wtcommon/idatacontrolplugin.h"

#include <cstdio>

struct WT_IDataControlPlugin
{
    int cnt = 0;
};

static WT_IDataControlPlugin* create_plugin()
{
    return new WT_IDataControlPlugin();
}

static void destroy_plugin(WT_IDataControlPlugin* obj)
{
    delete obj;
}

static void load_config(WT_IDataControlPlugin*, const char ***, int)
{

}

static int plugin_process(WT_IDataControlPlugin* obj,
                char[WT_MAX_APP_NAME_LEN],
                char[WT_MAX_WIN_TITLE_LEN],
                int*)
{
    obj->cnt++;
    printf("%d \n", obj->cnt);
    return 0;
}

WT_PLUGIN_DEFINE(
        1,
        1,
        "testowy",
        create_plugin,
        destroy_plugin,
        load_config,
        plugin_process)
