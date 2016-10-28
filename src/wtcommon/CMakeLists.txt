add_library(wtcommon
    datacontainer.cc
    datastorage.h
    filestorage.cc
    logger.cc)

target_link_libraries(wtcommon
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})
