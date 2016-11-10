if (UNIX)
    set(SYSTEM_SPECIFIC_SOURCES xwindowinfoprovider.cc gnomescreensaver.cc)
    set(SYSTEM_SPECIFIC_LIBRARIES ${GIO2_LIBRARIES})
elseif(WIN32)

endif(UNIX)

add_executable(workertracker
    main.cc
    workertracker.cc
    trackerjob.cc
    itracksuspendable.h
    daemonizer.cc
    windowinfoprovider.h
    backgroundrunner.h
    wtconstants.h
    ${SYSTEM_SPECIFIC_SOURCES})

target_link_libraries(workertracker
    wtcommon
    X11
    pthread
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${SYSTEM_SPECIFIC_LIBRARIES})

