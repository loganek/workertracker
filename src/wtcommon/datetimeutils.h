#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include <chrono>
#include <string>

namespace WT {

std::string time_to_display(std::chrono::seconds sec);

std::time_t seconds_from_epoch(const std::string& s);

}

#endif // DATETIMEUTILS_H
