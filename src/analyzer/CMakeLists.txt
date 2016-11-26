qt5_wrap_cpp(WTANALYZER_HDR_MOC qtanalyzerwindow.h graphdialog.h)
qt5_wrap_ui (WTANALYZER_UI_MOC  qtanalyzerwindow.ui graphdialog.ui)

add_executable(workertracker-analyzer
    main.cc
    analyzercontroller.cc
    imainwindow.h
    qtanalyzerwindow.cc
    qtfilterproxymodel.cc
    graphdialog.cc
    drilldownslice.cc
    drilldownchart.cc
    ${WTANALYZER_UI_MOC})

target_link_libraries(workertracker-analyzer
    wtcommon
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Charts_LIBRARIES})

set_property(TARGET workertracker-analyzer PROPERTY CXX_STANDARD 14)
