/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include <chrono>
#include <string>

namespace WT {

std::string time_to_display(std::chrono::seconds sec);

std::chrono::seconds seconds_from_epoch(const std::string& s);

}

#endif // DATETIMEUTILS_H
