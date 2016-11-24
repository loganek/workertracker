#include "datetimeutils.h"

#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace WT {

std::string time_to_display(std::chrono::seconds sec)
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
