/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "wtcommon/itracksuspendable.h"
#include "wtcommon/idatamodifier.h"

#include "wtcommon/logger.h"

#include <cstring>

class DummyTrackerPlugin : public WT::ITrackSuspendable
{
    bool silent = false;

protected:
    virtual ~DummyTrackerPlugin() {}

public:
    bool WT_APICALL suspend_tracking(const char *app, const char *title) override
    {
        if (!silent)
        {
            WT_LOG_D << "I'm not suspending, I'm just dummy plugin";
            WT_LOG_D << "APP: " << app << ", Title: " << title;
        }

        return false;
    }

    void WT_APICALL load_configuration(const char **config[2], int size) override
    {
        for (int i = 0; i < size; i++)
        {
            if (strcmp(config[0][i], "silent") == 0)
            {
                silent = std::atoi(config[1][i]);
                WT_LOG_D << "Loaded configuration 'silent': " << silent;
            }
        }
    }

    const char* WT_APICALL get_name() override { return "dummytrackerplugin"; }

    void WT_APICALL destroy() override
    {
        delete this;
    }
};

class DummyModifierPlugin : public WT::IDataModifier
{
protected:
    virtual ~DummyModifierPlugin() {}

public:
    virtual void WT_APICALL modify_data(
            char /*in_out_app_name*/[WT_MAX_APP_NAME_LEN],
            char /*in_out_window_title*/[WT_MAX_WIN_TITLE_LEN],
            bool &force_break) override
    {
        force_break = false;
    }

    void WT_APICALL load_configuration(const char **[2], int ) override
    {
    }

    int WT_APICALL get_rank() { return 999; }

    const char* WT_APICALL get_name() override { return "dummymodifierplugin"; }

    void WT_APICALL destroy() override
    {
        delete this;
    }
};

extern "C" WT_PLUGIN_EXPORT WT::ITrackSuspendable* WT_APICALL create_suspendable()
{
    return new DummyTrackerPlugin();
}


extern "C" WT_PLUGIN_EXPORT WT::IDataModifier* WT_APICALL create_data_modifier()
{
    return new DummyModifierPlugin();
}
