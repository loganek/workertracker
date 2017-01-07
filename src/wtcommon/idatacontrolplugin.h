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

/**
 * A struct that represents a plugin.
 */
struct WT_IDataControlPlugin;

/**
 * Creates an instance of the plugin.
 * @return The instance of the WT_IDataControlPlugin struct.
 */
typedef WT_IDataControlPlugin*(*WT_CreateFunc)();

/**
 * Destroys instance of the plugin.
 * @param plugin The instance of the WT_IDataControlPlugin struct.
 */
typedef void (*WT_DestroyFunc)(WT_IDataControlPlugin *plugin);

/**
 * Loads configuration to a plugin.
 * @param plugin The plugin.
 * @param config An array of key-value string pair that contains configuration for the plugin.
 * @param size A number of configuration entries.
 */
typedef void (*WT_LoadConfigFunc)(WT_IDataControlPlugin *plugin, const char **config[2], int size);

/**
 * Updates data entiers that will be stored in a database.
 * @param in_out_app_name An application name of the data entry.
 * @param in_out_window_title A window title of the data entry.
 * @return Zero, if other plugins should be processed; otherwise, non-zero value.
 */
typedef int (*WT_UpdateDataFunc)(
        WT_IDataControlPlugin*,
        char in_out_app_name[WT_MAX_APP_NAME_LEN],
        char in_out_window_title[WT_MAX_WIN_TITLE_LEN]);

/**
 * Returns information whether a tracking should be suspended.
 * @param app_name An application name of the data entry.
 * @param window_title A window title of the data entry.
 * @return Non-zero value, if the tracking should be suspended; otherwise, zero.
*/
typedef int (*WT_SuspendTrackingFunc)(
        WT_IDataControlPlugin*,
        const char app_name[WT_MAX_APP_NAME_LEN],
        const char window_title[WT_MAX_WIN_TITLE_LEN]);

typedef struct {
    int version;
    int rank;
    const char* name;
    WT_CreateFunc create_func;
    WT_DestroyFunc destroy_func;
    WT_LoadConfigFunc load_config_func;
    WT_UpdateDataFunc update_data_func;
    WT_SuspendTrackingFunc suspend_tracking_func;
} WT_PluginInfo;

#ifdef __cplusplus
}
#endif

/** Defines a plugin
  *
  * @param version A version of the workertracker that the plugin was compiled for.
  * @param rank A plugin priority rank - higher value, more important plugin.
  * @param name A name of the plugin.
  * @param create_func A pointer to the construction function of the WT_CreateFunc signature.
  * @param destroy_func A pointer to the destroy function of the WT_DestroyFunc signature.
  * @param load_config_func A pointer to the function that loads the configuration, of the WT_LoadConfigFunc signature, optional.
  * @param update_data_func A pointer to the function that might update a data, of the WT_UpdateDataFunc signature, optional.
  * @param suspend_tracking_func A pointer to the function that might suspend a tracking, of the WT_SuspendTrackingFunc signature, optional.
  */
#define WT_PLUGIN_DEFINE(version, rank, name, create_func, destroy_func, load_config_func, update_data_func, suspend_tracking_func) \
    WT_PLUGIN_EXPORT WT_PluginInfo wt_plugin_info = { \
        version, \
        rank, \
        name, \
        create_func, \
        destroy_func, \
        load_config_func, \
        update_data_func, \
        suspend_tracking_func \
    };


#endif // IDATACONTROLPLUGIN_H
