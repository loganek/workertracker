cmake_minimum_required(VERSION 2.8.7)

set(WT_PROJECT_NAME WorkerTracker)
string(TOLOWER ${WT_PROJECT_NAME} WT_CMAKE_PROJECT_NAME)

project(${WT_CMAKE_PROJECT_NAME})

set(WT_VERSION_MAJOR 0)
set(WT_VERSION_MINOR 1)
set(WT_VERSION_PATCH 0)
set(WT_FULL_VERSION "${WT_VERSION_MAJOR}.${WT_VERSION_MINOR}.${WT_VERSION_PATCH}")
add_definitions(-DWT_PROJECT_NAME_AND_VERSION="${WT_PROJECT_NAME} v${WT_FULL_VERSION}")
add_definitions(-DWT_PROJECT_NAME="${PROJECT_NAME}")

find_package(Boost 1.61 COMPONENTS system filesystem program_options REQUIRED)

if (UNIX)
    find_package (PkgConfig REQUIRED)
    pkg_check_modules (GIO2 REQUIRED gio-2.0)

    include_directories (${GIO2_INCLUDE_DIRS})
    link_directories (${GIO2_LIBRARY_DIRS})
elseif(WIN32)

endif(UNIX)

if (NOT DEFINED WITH_QT)
    set(WITH_QT ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++1y")

if(WITH_QT)
    message(STATUS "Use QT framework")
    find_package(Qt5Widgets)
    find_package(Qt5Charts)
    include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Charts_INCLUDE_DIRS})
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
else()
    message(WARNING "No UI library has been selected!")
endif(WITH_QT)


set(PROJECT_SOURCE_DIR "${workertracker_SOURCE_DIR}/src")
include_directories("${Boost_INCLUDE_DIRS}")

include_directories("${workertracker_SOURCE_DIR}/src")

add_subdirectory(src)

set(CPACK_COMPONENTS_ALL applications headers)
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "WorkerTracker Applications")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Plugin Headers")
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")
set(CPACK_PACKAGE_NAME "${WT_PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "www.cookandcommit.eu")
set(CPACK_PACKAGE_CONTACT "marcin.kolny@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "WorkerTracker - computer usage tracker")
set(CPACK_PACKAGE_VERSION "${WT_FULL_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${WT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${WT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${WT_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${WT_PROJECT_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)

include(CPack)
