#include "gtest/gtest.h"

#include "wtcommon/binaryexpressiontree.h"

using namespace WT;

std::shared_ptr<BinaryExpression> binary_expr_from_values(operand_value_t left, operand_value_t right, char op)
{
    return std::make_shared<BinaryExpression>(std::make_shared<ValueOperand>(left), std::make_shared<ValueOperand>(right), op);
}

TEST(BinaryExpressionTree, CheckPrecedenceForBinaryOperations)
{
    ASSERT_LT(BinaryExpression::get_operator_precedence('&'), BinaryExpression::get_operator_precedence('<'));
    ASSERT_LT(BinaryExpression::get_operator_precedence('&'), BinaryExpression::get_operator_precedence('>'));
    ASSERT_LT(BinaryExpression::get_operator_precedence('&'), BinaryExpression::get_operator_precedence('='));
    ASSERT_LT(BinaryExpression::get_operator_precedence('&'), BinaryExpression::get_operator_precedence('~'));
    ASSERT_LT(BinaryExpression::get_operator_precedence('&'), BinaryExpression::get_operator_precedence('!'));

    ASSERT_LT(BinaryExpression::get_operator_precedence('|'), BinaryExpression::get_operator_precedence('<'));
    ASSERT_LT(BinaryExpression::get_operator_precedence('|'), BinaryExpression::get_operator_precedence('>'));
    ASSERT_LT(BinaryExpression::get_operator_precedence('|'), BinaryExpression::get_operator_precedence('='));
    ASSERT_LT(BinaryExpression::get_operator_precedence('|'), BinaryExpression::get_operator_precedence('~'));
    ASSERT_LT(BinaryExpression::get_operator_precedence('|'), BinaryExpression::get_operator_precedence('!'));
}

TEST(BinaryExpressionTree, SingleBinaryOperationOfTheSameTypes_ShouldPass)
{
    auto expression = binary_expr_from_values(5L, 2L, '>');

    ASSERT_TRUE(expression->get_result());
}

TEST(BinaryExpressionTree, SingleBinaryOperationOfDifferentTypes_ShouldFail)
{
    auto expression = binary_expr_from_values(5L, true, '>');

    EXPECT_ANY_THROW(expression->get_result());
}

// "a" == "b" && 4 > 2
TEST(BinaryExpressionTree, AdvancedOperationTree_1)
{
    auto expression1 = binary_expr_from_values(std::string("a"), std::string("b"), '=');
    auto expression2 = binary_expr_from_values(4L, 2L, '>');

    auto out = std::make_shared<BinaryExpression>(expression1, expression2, '&');

    ASSERT_FALSE(out->get_result());
}

// "a" == "b" || 4 > 2
TEST(BinaryExpressionTree, AdvancedOperationTree_2)
{
    auto expression1 = binary_expr_from_values(std::string("a"), std::string("b"), '=');
    auto expression2 = binary_expr_from_values(4L, 2L, '>');

    auto out = std::make_shared<BinaryExpression>(expression1, expression2, '|');

    ASSERT_TRUE(out->get_result());
}

// ("a" == "b" && 4 > 2) || 5 == 4
TEST(BinaryExpressionTree, AdvancedOperationTree_3)
{
    auto expression1 = binary_expr_from_values(std::string("a"), std::string("b"), '=');
    auto expression2 = binary_expr_from_values(4L, 2L, '>');
    auto expression3 = binary_expr_from_values(5L, 4L, '=');

    auto out = std::make_shared<BinaryExpression>(
                std::make_shared<BinaryExpression>(expression1, expression2, '&'),
                expression3,
                '|');

    ASSERT_FALSE(out->get_result());
}

// ("a" == "b" && 4 > 2) || (5 == 4 || "testowy" ~ "t.*ow")
TEST(BinaryExpressionTree, AdvancedOperationTree_4)
{
    auto expression1 = binary_expr_from_values(std::string("a"), std::string("b"), '=');
    auto expression2 = binary_expr_from_values(4L, 2L, '>');
    auto expression3 = binary_expr_from_values(5L, 4L, '=');
    auto expression4 = binary_expr_from_values(std::string("testowy"), std::string("t.*ow"), '~');

    auto out = std::make_shared<BinaryExpression>(
                std::make_shared<BinaryExpression>(expression1, expression2, '&'),
                std::make_shared<BinaryExpression>(expression3, expression4, '|'),
                '|');

    ASSERT_TRUE(out->get_result());
}