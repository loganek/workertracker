/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "workertracker.h"

#include <clocale>

int main(int argc, char** argv)
{
    std::setlocale(LC_ALL, "");

    return WT::WorkerTracker::instance().run(argc, argv);
}
