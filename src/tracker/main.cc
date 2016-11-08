#include "workertracker.h"
#include "wtcommon/sqlitedataaccess.h"
int main(int argc, char** argv)
{
    WT::SQLiteDataAccess da("/home/loganek/file1.db");
    da.open();
    da.save_entry({time(nullptr), "aa", "b"});
    da.save_entry({time(nullptr)+1, "bb", "bb"});
    da.save_entry({time(nullptr)+2, "cc", "bbb"});
    da.save_entry({time(nullptr) + 3, "dd", "bbbb"});
    da.persist_records();
    return 0;
    return WT::WorkerTracker::instance().run(argc, argv);
}
