/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "analyzercontroller.h"

#include "wtcommon/logger.h"

int main(int argc, char **argv)
{
   if (auto controller = AnalyzerController::registry())
   {
       return controller->run(argc, argv);
   }
   else
   {
       WT_LOG(WT::LogLevel::EMERGENCY) << "No controller defined! Probably your operating system is not supported.";
       return 1;
   }
}
