include(ExternalProject)

ExternalProject_Add(
    chromietabsproject
    URL https://github.com/loganek/chromietabs/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/chromietabs
    INSTALL_COMMAND ""
    CMAKE_ARGS "-DCMAKE_CXX_FLAGS=-fPIC"
)

ExternalProject_Get_Property(chromietabsproject source_dir binary_dir)

add_library(libchromietabs IMPORTED STATIC GLOBAL)
add_dependencies(libchromietabs chromietabsproject)

find_library(LIBCHROMIETABS_LIBRARY NAMES chromietabs libchromietabs
    HINTS ${binary_dir})

set_target_properties(libchromietabs PROPERTIES
    "IMPORTED_LOCATION" "${LIBCHROMIETABS_LIBRARY}"
)

include_directories("${source_dir}/src/")
link_directories("${binary_dir}/src/chromietabs")

add_library(webbrowserurlreader
    SHARED
    webbrowserurlreader.cc)

add_dependencies(webbrowserurlreader libchromietabs)

set_property(TARGET webbrowserurlreader PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(webbrowserurlreader
    chromietabs)
