cmake_minimum_required(VERSION 2.8.9)

project(workertracker)

find_package(Boost 1.60 COMPONENTS filesystem program_options REQUIRED)

if (UNIX)
    find_package (PkgConfig REQUIRED)
    pkg_check_modules (GIO2 REQUIRED gio-2.0)
    pkg_check_modules (SQLITE REQUIRED sqlite3)

    include_directories (${GIO2_INCLUDE_DIRS})
    link_directories (${GIO2_LIBRARY_DIRS})
elseif(WIN32)
    # TODO find SQLITE3
endif(UNIX)

if (NOT DEFINED WITH_GTK AND NOT DEFINED WITH_QT)
    if (UNIX)
        set(WITH_GTK ON)
    elseif(WIN32)
        set(WITH_QT ON)
    endif()
endif()

if(WITH_GTK AND WITH_QT)
    message(FATAL_ERROR "Cannot use QT and GTK+ framework at the same time. Select only one!")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++1z")

if (WITH_GTK)
    message(STATUS "Use GTK+ framework")
    pkg_check_modules (GTKMM3 REQUIRED gtkmm-3.0)
    include_directories (${GTKMM3_INCLUDE_DIRS})
    link_directories (${GTKMM3_LIBRARY_DIRS})
elseif(WITH_QT)
    message(STATUS "Use QT framework")
    find_package(Qt5Widgets)
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
else()
    message(WARNING "No UI library has been selected!")
endif(WITH_GTK)


set(PROJECT_SOURCE_DIR "${workertracker_SOURCE_DIR}/src")
include_directories("${Boost_INCLUDE_DIRS}")

include_directories("${workertracker_SOURCE_DIR}/src")

add_subdirectory(src)
