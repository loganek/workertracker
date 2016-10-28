add_executable(workertracker
    main.cc
    xwindowinfoprovider.cc
    workertracker.cc
    trackerjob.cc
    itracksuspendable.h
    gnomescreensaver.cc
    daemonizer.cc
    registrable.h
    windowinfoprovider.h
    backgroundrunner.h
    wtconstants.h)

target_link_libraries(workertracker
    wtcommon
    X11
    pthread
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${GIO2_LIBRARIES})

