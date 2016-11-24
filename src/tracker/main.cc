/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "workertracker.h"

#ifndef WT_PROJECT_NAME
    #error WT_PROJECT_NAME macro has not been defined!
#endif

int main(int argc, char** argv)
{
    return WT::WorkerTracker::instance().run(argc, argv);
}
