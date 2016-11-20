add_executable(filtertokenizertest
    filtertokenizertest.cc
    ../src/analyzer/filtertokenizer.cc)

target_link_libraries(filtertokenizertest ${GTEST_BOTH_LIBRARIES})
add_test(FilterTokenizerTest filtertokenizertest)
