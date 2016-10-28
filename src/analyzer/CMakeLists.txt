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

add_executable(workertracker-analyzer
    main.cc
    analyzerwindow.cc
    ${RESOURCE_CC_FILE})

target_link_libraries(workertracker-analyzer
    wtcommon
    ${GTKMM3_LIBRARIES})


