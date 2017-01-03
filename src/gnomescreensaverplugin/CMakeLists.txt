add_library(gnomescreensaverplugin
    SHARED
    gnomescreensaver.cc)

target_link_libraries(gnomescreensaverplugin
    wtcommon)

install(TARGETS gnomescreensaverplugin
  RUNTIME
  DESTINATION bin/plugins
  LIBRARY
  DESTINATION bin/plugins
  ARCHIVE
  DESTINATION lib
  COMPONENT applications)
