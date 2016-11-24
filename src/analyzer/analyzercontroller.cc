/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "analyzercontroller.h"

#include "wtcommon/sqlitedataaccess.h"
#include "wtcommon/logger.h"
#include "wtcommon/configuration.h"

#include <boost/date_time.hpp>

int AnalyzerController::run(int, char **)
{
    main_window = construct_window();
    main_window->set_controller(this);

    WT::Configuration config(WT::Configuration::get_default_config_path());

    load_from_file(config.get_general_param("data-path").get());

    return 0;
}

void AnalyzerController::apply_filter(const std::string &search_text, bool case_sensitive)
{
    try
    {
        std::regex::flag_type flags = std::regex::ECMAScript;
        if (!case_sensitive)
        {
            flags |= std::regex::icase;
        }
        filter_pattern = search_text.empty() ? boost::none : boost::make_optional(std::regex(search_text, flags));
    }
    catch (const std::regex_error &ex)
    {
        WT_LOG(WT::LogLevel::WARNING) << ex.what();
        filter_pattern = boost::none;
    }
}

void AnalyzerController::load_from_file(const std::string &filename)
{
    try
    {
        data_access = std::make_shared<WT::SQLiteDataAccess>(filename);
        data_access->open(true);
        load_model(data_access->get_tree(period));
    }
    catch (const std::runtime_error &ex)
    {
        main_window->print_error(std::string("Error: ") + ex.what());
    }
}

void AnalyzerController::set_period(const WT::DateRange &period)
{
    this->period = period;
    if (data_access)
    {
        load_model(data_access->get_tree(period));
    }
}
