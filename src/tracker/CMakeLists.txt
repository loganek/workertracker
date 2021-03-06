if (UNIX)
    set(SYSTEM_SPECIFIC_SOURCES
        x11windowinfoprovider.cc
        posixsingleapplocker.cc
        daemonizer.cc)

    set(SYSTEM_SPECIFIC_LIBRARIES
        ${GIO2_LIBRARIES}
        dl
        X11
        pthread)
elseif(WIN32)
    set(SYSTEM_SPECIFIC_SOURCES
        mswindowsbgrunner.cc
        mswindowssingleapplocker.cc
        mswindowswindowinfoprovider.cc)
endif(UNIX)

add_executable(workertracker
    main.cc
    workertracker.cc
    trackerjob.cc
    pluginloader.cc
    plugincontainer.cc
    windowinfoprovider.h
    backgroundrunner.h
    singleapplocker.h
    ${SYSTEM_SPECIFIC_SOURCES})

target_link_libraries(workertracker
    wtcommon
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${SYSTEM_SPECIFIC_LIBRARIES})

install(TARGETS workertracker
  RUNTIME
  DESTINATION bin
  COMPONENT tracker)
