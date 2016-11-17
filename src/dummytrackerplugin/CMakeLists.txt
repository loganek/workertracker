add_library(dummytrackerplugin 
    SHARED
    dummytrackerplugin.cc)

target_link_libraries(dummytrackerplugin
    wtcommon)
