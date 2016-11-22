#include "workertracker.h"

#ifndef WT_PROJECT_NAME
    #error WT_PROJECT_NAME macro has not been defined!
#endif

int main(int argc, char** argv)
{
    return WT::WorkerTracker::instance().run(argc, argv);
}
