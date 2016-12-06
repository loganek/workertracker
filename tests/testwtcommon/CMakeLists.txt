file(GLOB SRCS *.cc)

ADD_EXECUTABLE(testwtcommon ${SRCS})

TARGET_LINK_LIBRARIES(testwtcommon
    wtcommon
    libgtest
    libgmock
)

add_test(NAME testwtcommon COMMAND testwtcommon)
