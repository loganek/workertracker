#include "analyzercontroller.h"

#include "wtcommon/sqlitedataaccess.h"
#include "wtcommon/logger.h"

#include <boost/date_time.hpp>

int AnalyzerController::run(int, char **)
{
    main_window = construct_window();
    main_window->set_controller(this);

    load_from_file("/home/loganek/.workertracker/data.dat");

    return 0;
}

std::string AnalyzerController::time_to_display(std::chrono::seconds sec)
{
    using namespace std::chrono;
    std::ostringstream ss;

    using namespace std::literals::chrono_literals;

    auto h = duration_cast<hours>(sec);
    if (h > 0h)
    {
        sec -= h;
        ss << h.count() << "h ";
    }
    auto m = duration_cast<minutes>(sec);
    if (m > 0min || h > 0h)
    {
        sec -= m;
        ss << m.count() << "min ";
    }
    ss << sec.count() << "s";
    return ss.str();
}

std::time_t AnalyzerController::seconds_from_epoch(const std::string& s)
{
    namespace bt = boost::posix_time;
    std::locale format(std::locale::classic(), new bt::time_input_facet("%Y/%m/%d %H:%M:%S"));

    std::istringstream is(s);
    is.imbue(format);
    bt::ptime pt;
    is >> pt;
    bt::ptime timet_start(boost::gregorian::date(1970,1,1));
    bt::time_duration diff = pt - timet_start;

    return diff.ticks() / bt::time_duration::rep_type::ticks_per_second;
}

void AnalyzerController::on_search(const std::string &search_text)
{
    try
    {
        filter_pattern = search_text.empty() ? boost::none : boost::make_optional(std::regex(search_text));
    }
    catch (const std::regex_error &ex)
    {
        WT_LOG(WT::LogLevel::WARNING) << ex.what();
        filter_pattern = boost::none;
    }

    apply_filter();
}

void AnalyzerController::load_from_file(const std::string &filename)
{
    try
    {
        data_access = std::make_shared<WT::SQLiteDataAccess>(filename);
        data_access->open(true);
        load_model_to_view(data_access->get_tree());
    }
    catch (const std::runtime_error &ex)
    {
        main_window->print_error(std::string("Error: ") + ex.what());
    }
}
