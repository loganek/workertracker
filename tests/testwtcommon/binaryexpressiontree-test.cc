/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "gtest/gtest.h"

#include "wtcommon/binaryexpressiontree.h"

using namespace WT;

std::shared_ptr<BinaryExpression> binary_expr_from_values(operand_value_t left, operand_value_t right, Operator op)
{
    return std::make_shared<BinaryExpression>(std::make_shared<ValueOperand>(left), std::make_shared<ValueOperand>(right), op);
}

TEST(BinaryExpressionTree, CheckPrecedenceForBinaryOperations)
{
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::AND), BinaryExpression::get_operator_precedence(Operator::GT));
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::AND), BinaryExpression::get_operator_precedence(Operator::LT));
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::AND), BinaryExpression::get_operator_precedence(Operator::EQ));
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::AND), BinaryExpression::get_operator_precedence(Operator::MATCH));
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::AND), BinaryExpression::get_operator_precedence(Operator::NEQ));

    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::OR), BinaryExpression::get_operator_precedence(Operator::GT));
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::OR), BinaryExpression::get_operator_precedence(Operator::LT));
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::OR), BinaryExpression::get_operator_precedence(Operator::EQ));
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::OR), BinaryExpression::get_operator_precedence(Operator::MATCH));
    ASSERT_LT(BinaryExpression::get_operator_precedence(Operator::OR), BinaryExpression::get_operator_precedence(Operator::NEQ));
}

TEST(BinaryExpressionTree, SingleBinaryOperationOfTheSameTypes_ShouldPass)
{
    auto expression = binary_expr_from_values(5, 2, Operator::GT);

    ASSERT_TRUE(expression->get_result());
}

TEST(BinaryExpressionTree, SingleBinaryOperationOfDifferentTypes_ShouldFail)
{
    auto expression = binary_expr_from_values(5, true, Operator::GT);

    EXPECT_ANY_THROW(expression->get_result());
}

// "a" == "b" && 4 > 2
TEST(BinaryExpressionTree, AdvancedOperationTree_1)
{
    auto expression1 = binary_expr_from_values(std::string("a"), std::string("b"), Operator::EQ);
    auto expression2 = binary_expr_from_values(4, 2, Operator::GT);

    auto out = std::make_shared<BinaryExpression>(expression1, expression2, Operator::AND);

    ASSERT_FALSE(out->get_result());
}

// "a" == "b" || 4 > 2
TEST(BinaryExpressionTree, AdvancedOperationTree_2)
{
    auto expression1 = binary_expr_from_values(std::string("a"), std::string("b"), Operator::EQ);
    auto expression2 = binary_expr_from_values(4, 2, Operator::GT);

    auto out = std::make_shared<BinaryExpression>(expression1, expression2, Operator::OR);

    ASSERT_TRUE(out->get_result());
}

// ("a" == "b" && 4 > 2) || 5 == 4
TEST(BinaryExpressionTree, AdvancedOperationTree_3)
{
    auto expression1 = binary_expr_from_values(std::string("a"), std::string("b"), Operator::EQ);
    auto expression2 = binary_expr_from_values(4, 2, Operator::GT);
    auto expression3 = binary_expr_from_values(5, 4, Operator::EQ);

    auto out = std::make_shared<BinaryExpression>(
                std::make_shared<BinaryExpression>(expression1, expression2, Operator::AND),
                expression3,
                Operator::OR);

    ASSERT_FALSE(out->get_result());
}

// ("a" == "b" && 4 > 2) || (5 == 4 || "testowy" ~ "t.*ow")
TEST(BinaryExpressionTree, AdvancedOperationTree_4)
{
    auto expression1 = binary_expr_from_values(std::string("a"), std::string("b"), Operator::EQ);
    auto expression2 = binary_expr_from_values(4, 2, Operator::GT);
    auto expression3 = binary_expr_from_values(5, 4, Operator::EQ);
    auto expression4 = binary_expr_from_values(std::string("testowy"), std::string("t.*ow"), Operator::MATCH);

    auto out = std::make_shared<BinaryExpression>(
                std::make_shared<BinaryExpression>(expression1, expression2, Operator::AND),
                std::make_shared<BinaryExpression>(expression3, expression4, Operator::OR),
                Operator::OR);

    ASSERT_TRUE(out->get_result());
}
