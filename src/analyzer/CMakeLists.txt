if (WITH_GTK)
    find_program(GLIB_COMPILE_RESOURCES_EXECUTABLE glib-compile-resources)

    set(RESOURCE_CC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources.cc)
    set(RESOURCE_XML_FILE data/eu.cookandcommit.workertracker.analyzer.xml)

    file(GLOB resource_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/data" "data/*.glade")
    set(RES_DEPENDENCIES "")
    foreach(file ${resource_files})
        list(APPEND RES_DEPENDENCIES "${CMAKE_CURRENT_SOURCE_DIR}/data/${file}")
    endforeach()

    add_custom_command(
        PRE_BUILD
        OUTPUT
            ${RESOURCE_CC_FILE}
        COMMAND
            ${GLIB_COMPILE_RESOURCES_EXECUTABLE}
        ARGS
            "--generate-source"
            "--target=${RESOURCE_CC_FILE}"
            "--sourcedir=data"
            ${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_XML_FILE}
        MAIN_DEPENDENCY
            ${RESOURCE_XML_FILE}
        DEPENDS
            ${RES_DEPENDENCIES}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_SOURCE_DIR})

    set(FRAMEWORK_SPECIFIC_SOURCES analyzerwindow.cc gtkanalyzercontroller.cc ${RESOURCE_CC_FILE})
    set(FRAMEWORK_SPECIFIC_LIBRARIES ${GTKMM3_LIBRARIES})

elseif(WITH_QT)
    qt5_wrap_cpp(WTANALYZER_HDR_MOC qtanalyzerwindow.h qtconfigurationdialog.h)
    qt5_wrap_ui (WTANALYZER_UI_MOC  qtanalyzerwindow.ui qtconfigurationdialog.ui)
    set(FRAMEWORK_SPECIFIC_SOURCES qtanalyzercontroller.cc
        qtanalyzerwindow.cc
        qtconfigurationdialog.cc
        qtfilterproxymodel.cc
        ${WTANALYZER_UI_MOC})
    set(FRAMEWORK_SPECIFIC_LIBRARIES ${Qt5Widgets_LIBRARIES})
endif(WITH_GTK)

add_executable(workertracker-analyzer
    main.cc
    analyzercontroller.cc
    filtertokenizer.cc
    imainwindow.h
    filterexpressionparser.cc
    ${FRAMEWORK_SPECIFIC_SOURCES})

target_link_libraries(workertracker-analyzer
    wtcommon
    ${FRAMEWORK_SPECIFIC_LIBRARIES})

set_property(TARGET workertracker-analyzer PROPERTY CXX_STANDARD 14)
