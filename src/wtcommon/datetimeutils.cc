/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "datetimeutils.h"

#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace WT {

std::string time_to_display(std::chrono::seconds sec)
{
    using namespace std::chrono;
    std::ostringstream ss;
    auto sec_init = sec;

    using namespace std::literals::chrono_literals;

    auto h = duration_cast<hours>(sec);
    if (h > 0h)
    {
        sec -= h;
        ss << h.count() << "h ";
    }
    auto m = duration_cast<minutes>(sec);
    if (m > 0min)
    {
        sec -= m;
        ss << m.count() << "min ";
    }
    if (sec.count() > 0 || sec_init == 0s)
    {
        ss << sec.count() << "s";
    }
    return ss.str();
}

std::chrono::seconds seconds_from_epoch(const std::string& s)
{
    namespace bt = boost::posix_time;
    std::locale format(std::locale::classic(), new bt::time_input_facet("%Y/%m/%d %H:%M:%S"));

    std::istringstream is(s);
    is.imbue(format);
    bt::ptime pt;
    is >> pt;
    bt::ptime timet_start(boost::gregorian::date(1970,1,1));
    bt::time_duration diff = pt - timet_start;

    return std::chrono::seconds(diff.ticks() / bt::time_duration::rep_type::ticks_per_second);
}

}
