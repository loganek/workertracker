cmake_minimum_required(VERSION 2.8.7)

project(workertracker)

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
