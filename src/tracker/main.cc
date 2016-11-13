#include "workertracker.h"

int main(int argc, char** argv)
{
    return WT::WorkerTracker::instance().run(argc, argv);
}
