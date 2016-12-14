if (UNIX)
    set(SYSTEM_SPECIFIC_SOURCES
        unixsyslogloggermethod.cc)
    set(BUILD_FLAGS -fPIC)
elseif(WIN32)

endif(UNIX)

add_library(wtcommon
    datacontainer.h
    dataaccess.h
    dataentry.h
    sqlitedataaccess.cc
    logger.cc
    itracksuspendable.h
    configuration.cc
    dataaccess.cc
    binaryexpressiontree.cc
    binaryexpressionparser.cc
    registrable.h
    datetimeutils.cc
    fileloggermethod.cc
    ${SYSTEM_SPECIFIC_SOURCES})

if(DEFINED BUILD_FLAGS)
    set_target_properties(wtcommon PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
endif()
    
target_link_libraries(wtcommon
    sqlite3
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})

install(FILES itracksuspendable.h
  DESTINATION include
  COMPONENT headers)
