/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef IDATACONTROLPLUGIN_H
#define IDATACONTROLPLUGIN_H

#ifdef WIN32
# define WT_PLUGIN_EXPORT __declspec(dllexport)
#else
# define WT_PLUGIN_EXPORT // empty
#endif

// TODO defined in sqlitedataaccess too..
#define WT_MAX_APP_NAME_LEN 50
#define WT_MAX_WIN_TITLE_LEN 150

#ifdef __cplusplus
extern "C" {
#endif

struct WT_IDataControlPlugin;

typedef WT_IDataControlPlugin*(*WT_CreateFunc)();
typedef void (*WT_DestroyFunc)(WT_IDataControlPlugin *);
typedef void (*WT_LoadConfigFunc)(WT_IDataControlPlugin*, const char **config[2], int size);

typedef int (*WT_ControlDataFunc)(
        WT_IDataControlPlugin*,
        char in_out_app_name[WT_MAX_APP_NAME_LEN],
        char in_out_window_title[WT_MAX_WIN_TITLE_LEN],
        int* out_force_break);

typedef struct {
    int version;
    int rank;
    const char* name;
    WT_CreateFunc create_func;
    WT_DestroyFunc destroy_func;
    WT_LoadConfigFunc load_config_func;
    WT_ControlDataFunc control_data_func;
} WT_PluginInfo;

#ifdef __cplusplus
}
#endif


#define WT_PLUGIN_DEFINE(version, rank, name, create_func, destroy_func, load_config_func, control_data_func) \
    WT_PLUGIN_EXPORT WT_PluginInfo wt_plugin_info = { \
        version, \
        rank, \
        name, \
        create_func, \
        destroy_func, \
        load_config_func, \
        control_data_func \
    };


#endif // IDATACONTROLPLUGIN_H
