#include "wtcommon/idatacontrolplugin.h"

#include <chromietabs/pathutils.h>
#include <chromietabs/sessionanalyzer.h>

#include <boost/algorithm/string/predicate.hpp>

#include <codecvt>
#include <regex>
#include <cstring>

struct WT_IDataControlPlugin
{
};

static WT_IDataControlPlugin* create_plugin()
{
    return new WT_IDataControlPlugin();
}

static void destroy_plugin(WT_IDataControlPlugin* obj)
{
    delete obj;
}

static std::string get_chrome_url(const char* window_title, ChromieTabs::BrowserType browser)
{
    ChromieTabs::SessionAnalyzer analyzer{ChromieTabs::SessionReader(ChromieTabs::PathUtils::get_current_session_path(browser))};
    std::string url;
    auto win_id = analyzer.get_current_window_id();

    if (win_id < 0)
    {
        return url;
    }

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    try
    {
        const auto& navigation_entry = analyzer.get_current_tab(win_id).get_current_navigation_entry();
        std::string title = convert.to_bytes(navigation_entry.title);
        // google-chrome might add - Google Chrome at the end of the title to the application
        // title, so I use starts_with() instead of operator==().
        if (boost::starts_with(window_title, title))
        {
            url = navigation_entry.url;
        }
    }
    catch (const std::out_of_range&)
    {
    }
    return url;
}

static bool get_url(const char *app_name, const char *window_title, std::string &out_url)
{
    if (strcmp(app_name, "google-chrome") == 0)
    {
        out_url = get_chrome_url(window_title, ChromieTabs::BrowserType::GOOGLE_CHROME);
        return !out_url.empty();
    }
    else if (strcmp(app_name, "chromium-browser") == 0)
    {
        out_url = get_chrome_url(window_title, ChromieTabs::BrowserType::CHROMIUM);
        return !out_url.empty();
    }

    return false;
}

static bool get_domain(std::string url, std::string& out_domain)
{
    std::regex domain_pattern("^(?:https?:\\/\\/)?(?:[^@\\/\\n]+@)?(?:www\\.)?([^:\\/\\n]+)");
    std::smatch domain_match;

    if(std::regex_search(url, domain_match, domain_pattern) && domain_match.size() >= 2)
    {
        out_domain = domain_match[1];
        return true;
    }

    return false;
}

static int plugin_update_data(WT_IDataControlPlugin* ,
                char app_name[WT_MAX_APP_NAME_LEN],
                char window_title[WT_MAX_WIN_TITLE_LEN])
{
    std::string url;
    if (!get_url(app_name, window_title, url))
    {
        return 0;
    }

    std::string domain;
    if (!get_domain(url, domain))
    {
        return 0;
    }

    char tmp_win_title[WT_MAX_WIN_TITLE_LEN] = {0};
    strncpy(tmp_win_title, window_title, WT_MAX_APP_NAME_LEN);
    snprintf(window_title, WT_MAX_WIN_TITLE_LEN, "[web: %s] %s", domain.c_str(), tmp_win_title);

    return 0;
}

WT_PLUGIN_DEFINE(
        0,
        1,
        "webbrowserurlreader",
        create_plugin,
        destroy_plugin,
        nullptr,
        plugin_update_data,
        nullptr)
