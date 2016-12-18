#include "gtest/gtest.h"

#include "wtcommon/binaryexpressionparser.h"

using namespace WT;

TEST(BinaryExpressionParser, SingleValue_ShouldReturnNullptr)
{
    ASSERT_TRUE(!BinaryExpressionParser("5").parse());
}

TEST(BinaryExpressionParser, SingleBinaryExpression)
{
    ASSERT_TRUE(BinaryExpressionParser("7 > 5").parse()->get_result());
}

TEST(BinaryExpressionParser, InvalidSyntax_NoRightOperand_ShouldFail)
{
    EXPECT_ANY_THROW(BinaryExpressionParser("7 > ").parse());
}

TEST(BinaryExpressionParser, InvalidSyntax_NoLeftOperand_ShouldFail)
{
    EXPECT_ANY_THROW(BinaryExpressionParser("> 6").parse());
}

TEST(BinaryExpressionParser, InvalidSyntax_NoOperand_ShouldFail)
{
    EXPECT_ANY_THROW(BinaryExpressionParser(">").parse());
}

TEST(BinaryExpressionParser, InvalidSyntax_NoOperator_ShouldFail)
{
    EXPECT_ANY_THROW(BinaryExpressionParser("5 6").parse());
}

TEST(BinaryExpressionParser, InvalidSyntax_NoClosingParetnhesis_ShouldFail)
{
    EXPECT_ANY_THROW(BinaryExpressionParser("(5 < 6").parse());
}

TEST(BinaryExpressionParser, InvalidSyntax_NoOpeningParetnhesis_ShouldFail)
{
    EXPECT_ANY_THROW(BinaryExpressionParser("5 < 6)").parse());
}

TEST(BinaryExpressionParser, AdvancedExpression_1)
{
    ASSERT_TRUE(BinaryExpressionParser("(5 < 6) & \"abcdef\"~\"^.b.{2}ef$\"").parse()->get_result());
}

TEST(BinaryExpressionParser, AdvancedExpression_2)
{
    ASSERT_TRUE(BinaryExpressionParser("(5 < 6) & 4 < 12 | (7 > 9 & 2 < 0)").parse()->get_result());
}

TEST(BinaryExpressionParser, AdvancedExpression_3)
{
    ASSERT_TRUE(BinaryExpressionParser("(((5 < 6) & 4 < 12) & 5 < 4) | ((7 > 9 & 2 < 0) | 4 > 2)").parse()->get_result());
}
