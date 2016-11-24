add_subdirectory(sqlite3)
add_subdirectory(wtcommon)
add_subdirectory(tracker)
add_subdirectory(dummytrackerplugin)

if (WITH_QT)
    add_subdirectory(analyzer)
endif(WITH_QT)
