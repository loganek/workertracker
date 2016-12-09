file(GLOB SRCS *.cc)

add_executable(testwtcommon ${SRCS})

target_link_libraries(testwtcommon
    wtcommon
    libgtest
    libgmock
    gtest_main
)

add_test(NAME testwtcommon COMMAND testwtcommon)
