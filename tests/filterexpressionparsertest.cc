#include "analyzer/filterexpressionparser.h"

#include <gtest/gtest.h>

TEST(FilterTokenizerTest, CheckUnaryOperations)
{
    ASSERT_TRUE(FilterExpressionParser("4").get_result());
    ASSERT_FALSE(FilterExpressionParser("0").get_result());

    ASSERT_TRUE(FilterExpressionParser("\"test\"").get_result());
    ASSERT_FALSE(FilterExpressionParser("\"\"").get_result());

    FilterExpressionParser p1("var");
    p1.add_variable("var", 0);
    ASSERT_FALSE(p1.get_result());

    FilterExpressionParser p2("var");
    p2.add_variable("var", 4);
    ASSERT_TRUE(p2.get_result());

    FilterExpressionParser p3("var");
    p3.add_variable("var", std::string(""));
    ASSERT_FALSE(p3.get_result());

    FilterExpressionParser p4("var");
    p4.add_variable("var", std::string("test"));
    ASSERT_TRUE(p4.get_result());
}

TEST(FilterTokenizerTest, CheckComparisionBetweenTheSameTypes)
{
    ASSERT_TRUE(FilterExpressionParser("12 < 13").get_result());
    ASSERT_FALSE(FilterExpressionParser("12 = 3").get_result());

    ASSERT_TRUE(FilterExpressionParser("\"test\" = \"test\"").get_result());
    ASSERT_FALSE(FilterExpressionParser("\"test\" = \"none\"").get_result());
}

TEST(FilterTokenizerTest, CheckComparisionBetweenVariables)
{
    FilterExpressionParser p1("int5 < int7");
    p1.add_variable("int5", 5);
    p1.add_variable("int7", 7);
    ASSERT_TRUE(p1.get_result());

    FilterExpressionParser p2("int5 > int7");
    p2.add_variable("int5", 5);
    p2.add_variable("int7", 7);
    ASSERT_FALSE(p2.get_result());
}

TEST(FilterTokenizerTest, UseUndeclaredVariables_ShouldThrow)
{
    FilterExpressionParser p("var > 5");
    EXPECT_THROW(p.get_result(), std::runtime_error);
}

TEST(FilterTokenizerTest, InvalidTypeOfVariable_ShouldFail)
{
    FilterExpressionParser p("var = 5");
    p.add_variable("var", std::string("\"string\""));
    ASSERT_THROW(p.get_result(), std::runtime_error);
}

TEST(FilterTokenizerTest, CombinedExpressions)
{
    FilterExpressionParser p("var < 6 & \"a\" = \"a\"");
    p.add_variable("var", 5);
    ASSERT_TRUE(p.get_result());

    ASSERT_TRUE(FilterExpressionParser("12 < 13 & \"test\" = \"test\"").get_result());
    ASSERT_FALSE(FilterExpressionParser("9 < 6 & 2 < 8").get_result());
    ASSERT_TRUE(FilterExpressionParser("12 < 13 & \"test\" = \"test\" & 4 < 6").get_result());
    ASSERT_FALSE(FilterExpressionParser("12 < 13 & \"test\" = \"x\" & 4 < 6").get_result());
}
