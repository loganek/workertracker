cmake_minimum_required(VERSION 2.8.9)

project(workertracker)

find_package(Boost 1.60 COMPONENTS filesystem program_options REQUIRED)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GIO2 REQUIRED gio-2.0)
pkg_check_modules (GTKMM3 REQUIRED gtkmm-3.0)
pkg_check_modules (SQLITE REQUIRED sqlite3)

include_directories (${GIO2_INCLUDE_DIRS})
link_directories (${GIO2_LIBRARY_DIRS})

include_directories (${GTKMM3_INCLUDE_DIRS})
link_directories (${GTKMM3_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++1y")
set(PROJECT_SOURCE_DIR "${workertracker_SOURCE_DIR}/src")
include_directories("${Boost_INCLUDE_DIRS}")

include_directories("${workertracker_SOURCE_DIR}/src")

add_subdirectory(src)
