qt5_wrap_cpp(WTANALYZER_HDR_MOC qtanalyzerwindow.h graphdialog.h)
qt5_wrap_ui (WTANALYZER_UI_MOC  qtanalyzerwindow.ui graphdialog.ui aboutdialog.ui)

add_executable(workertracker-analyzer
    main.cc
    analyzercontroller.cc
    imainwindow.h
    qtanalyzerwindow.cc
    qtfilterproxymodel.cc
    graphdialog.cc
    drilldownslice.cc
    drilldownchart.cc
    smartpieseries.cc
    ${WTANALYZER_UI_MOC})

target_link_libraries(workertracker-analyzer
    wtcommon
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Charts_LIBRARIES})

set_property(TARGET workertracker-analyzer PROPERTY CXX_STANDARD 14)

if(WIN32)
  add_custom_command(
    TARGET workertracker-analyzer
    POST_BUILD
    COMMAND windeployqt.exe $<TARGET_FILE:workertracker-analyzer>
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR} 
    COMMENT "Deploy qt5 files")

  install(DIRECTORY $<TARGET_FILE_DIR:workertracker-analyzer>/
    DESTINATION bin
    COMPONENT analyzer)
else()
  install(TARGETS workertracker-analyzer
    RUNTIME
    DESTINATION bin
    COMPONENT analyzer)
endif(WIN32)

