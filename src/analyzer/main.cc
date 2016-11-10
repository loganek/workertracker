#include "analyzercontroller.h"

#include "wtcommon/logger.h"

int main(int argc, char **argv)
{
    WT::MethodOutput::set_method(WT::StdOutput::output);

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
