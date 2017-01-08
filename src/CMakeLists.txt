add_subdirectory(sqlite3)
add_subdirectory(wtcommon)
add_subdirectory(tracker)
add_subdirectory(dummytrackerplugin)

if (UNIX)
    add_subdirectory(gnomescreensaverplugin)
    add_subdirectory(webbrowserurlreader)
endif(UNIX)

if (WITH_QT)
    add_subdirectory(analyzer)
endif(WITH_QT)
