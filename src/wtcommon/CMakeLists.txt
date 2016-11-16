if (UNIX)
    set(SYSTEM_SPECIFIC_SOURCES
        unixsyslogloggermethod.cc)
elseif(WIN32)

endif(UNIX)

add_library(wtcommon
    datacontainer.cc
    datastorage.h
#    filestorage.cc TODO
    dataaccess.h
    sqlitedataaccess.cc
    logger.cc
    itracksuspendable.h
    configuration.cc
    registrable.h
    ${SYSTEM_SPECIFIC_SOURCES})

target_link_libraries(wtcommon
    sqlite3
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})
