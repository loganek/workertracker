cmake_minimum_required(VERSION 2.8.7)

set(WT_PROJECT_NAME WorkerTracker)
string(TOLOWER ${WT_PROJECT_NAME} WT_CMAKE_PROJECT_NAME)

project(${WT_CMAKE_PROJECT_NAME})

set(WT_AUTHOR_URL "http://www.cookandcommit.eu")
set(WT_AUTHOR_MAIL "marcin.kolny@gmail.com")
set(WT_VERSION_MAJOR 0)
set(WT_VERSION_MINOR 3)
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

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

if(WITH_QT)
    message(STATUS "Use QT framework")
    find_package(Qt5Widgets)
    find_package(Qt5Charts)
    find_package(Qt5Core)
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

enable_testing()

add_subdirectory(src)

if (UNIX)
    add_subdirectory(tests)
endif()

################### INSTALLER ###################

if(WIN32)
  #### NSIS ###
  set(CPACK_GENERATOR NSIS)

  set(CPACK_NSIS_CONTACT "${WT_AUTHOR_MAIL}")
  set(CPACK_NSIS_PACKAGE_NAME "${WT_PROJECT_NAME}")
  set(CPACK_NSIS_DISPLAY_NAME "${WT_PROJECT_NAME}")
  set(CPACK_NSIS_MODIFY_PATH "ON")
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")
  set(CPACK_NSIS_HELP_LINK "${WT_AUTHOR_URL}")
  set(CPACK_NSIS_URL_INFO_ABOUT "${WT_AUTHOR_URL}")
  set(CPACK_NSIS_TRACKER "workertracker")
endif(WIN32)

set(CPACK_SOURCE_IGNORE_FILES "build")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_EXECUTABLES "workertracker-analyzer" "${WT_PROJECT_NAME} Analyzer")
set(CPACK_COMPONENTS_ALL tracker analyzer headers)
set(CPACK_COMPONENT_TRACKER_DISPLAY_NAME "Tracking Application")
set(CPACK_COMPONENT_ANALYZER_DISPLAY_NAME "Analyzer Tool")
set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Plugin Headers")
set(CPACK_COMPONENT_TRACKER_GROUP "Runtime")
set(CPACK_COMPONENT_ANALYZER_GROUP "Runtime")
set(CPACK_COMPONENT_HEADERS_GROUP "Development")
set(CPACK_PACKAGE_NAME "${WT_PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Marcin Kolny (${WT_AUTHOR_URL})")
set(CPACK_PACKAGE_CONTACT "${WT_AUTHOR_MAIL}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "WorkerTracker - computer usage tracker")
set(CPACK_PACKAGE_VERSION "${WT_FULL_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${WT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${WT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${WT_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${WT_PROJECT_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/LICENSE)

set(CPACK_MODULE_PATH "${workertracker_SOURCE_DIR}/nsis")

include(CPack)
