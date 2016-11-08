add_library(wtcommon
    datacontainer.cc
    datastorage.h
    filestorage.cc
    dataaccess.h
    sqlitedataaccess.cc
    logger.cc)

target_link_libraries(wtcommon
    ${SQLITE_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})
