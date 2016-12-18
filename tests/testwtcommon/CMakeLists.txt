file(GLOB SRCS *.cc)

add_executable(testwtcommon ${SRCS})

target_link_libraries(testwtcommon
    wtcommon
    libgtest
)

add_test(NAME testwtcommon COMMAND testwtcommon)
