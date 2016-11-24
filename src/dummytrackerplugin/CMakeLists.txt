add_library(dummytrackerplugin 
    SHARED
    dummytrackerplugin.cc)

target_link_libraries(dummytrackerplugin
    wtcommon)

install(TARGETS dummytrackerplugin
  RUNTIME
  DESTINATION bin/plugins
  LIBRARY
  DESTINATION bin/plugins
  ARCHIVE
  DESTINATION lib
  COMPONENT applications)
