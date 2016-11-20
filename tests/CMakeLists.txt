add_executable(filtertokenizertest
    filtertokenizertest.cc
    ../src/analyzer/filtertokenizer.cc)

add_executable(filterexpressionparsertest
    filterexpressionparsertest.cc
    ../src/analyzer/filtertokenizer.cc
    ../src/analyzer/filterexpressionparser.cc)

target_link_libraries(filtertokenizertest ${GTEST_BOTH_LIBRARIES})
add_test(FilterTokenizerTest filtertokenizertest)

target_link_libraries(filterexpressionparsertest ${GTEST_BOTH_LIBRARIES})
add_test(FilterExpressionParserTest filtertokenizertest)
