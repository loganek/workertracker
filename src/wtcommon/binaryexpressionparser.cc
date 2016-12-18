/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "binaryexpressionparser.h"

#include <sstream>
#include <unordered_map>
#include <iomanip>

namespace WT {

static int precedence(Operator op)
{
    return BinaryExpression::get_operator_precedence(op);
}

BinaryExpressionParser::BinaryExpressionParser(const std::string &expression_str, const std::unordered_map<std::string, operand_value_t>& variables)
    : variables(variables), tokenizer(expression_str)
{
}

std::shared_ptr<ValueOperand> BinaryExpressionParser::get_datetime_from_string(const std::string& str)
{
    std::istringstream ss(str);
    const std::string date_format = "%Y-%m-%d %H:%M:%S";
    std::tm tm_struct = {};
    ss >> std::get_time(&tm_struct, date_format.c_str());
    if (ss.fail())
    {
        throw std::runtime_error("invalid date format, expected: " + date_format);
    }

    return std::make_shared<ValueOperand>(tm_struct);
}

static Operator translate_operator(const std::string &op)
{
    static std::unordered_map<std::string, Operator> translation = {
        {"!", Operator::NEQ},
        {"=", Operator::EQ},
        {">", Operator::GT},
        {">=", Operator::GE},
        {"<", Operator::LT},
        {"<=", Operator::LE},
        {"~", Operator::MATCH},
        {"&", Operator::AND},
        {"|", Operator::OR},
        {"(", Operator::OPEN_PARENTHESIS},
        {")", Operator::CLOSE_PARENTHESIS}
    };

    auto it = translation.find(op);
    if (it == translation.end())
    {
        throw std::runtime_error("unknown operator: " + op);
    }
    return it->second;
}

void BinaryExpressionParser::read_operator()
{
    assert(current_token->type == TokenType::OPERATOR);

    auto op = translate_operator(current_token->value);

    if (op == Operator::OPEN_PARENTHESIS)
    {
        operators.push(op);
    }
    else if (op == Operator::CLOSE_PARENTHESIS)
    {
        while (!operators.empty() && operators.top() != Operator::OPEN_PARENTHESIS)
        {
            make_binary_expression();
        }
        operators.pop();
    }
    else
    {
        int op_precendence = precedence(op);
        while ((!operators.empty()) && (op_precendence <= precedence(operators.top())))
        {
            make_binary_expression();
        }
        operators.push(op);
    }
}

void BinaryExpressionParser::read_identifier()
{
    auto it = variables.find(current_token->value);

    if (it == variables.end())
    {
        throw std::runtime_error("undefined identifier " + current_token->value);
    }

    operands.push(std::make_shared<VariableOperand>(current_token->value, it->second));
}

void BinaryExpressionParser::read_string(const std::vector<Token>::iterator& last)
{
    std::shared_ptr<ValueOperand> operand;
    auto next_it = std::next(current_token);

    if (next_it != last && next_it->type == TokenType::IDENTIFIER && next_it->value == "d")
    {
        operand = get_datetime_from_string(current_token->value);
        current_token = next_it;
    }
    else
    {
        operand = std::make_shared<ValueOperand>(current_token->value);
    }

    operands.push(operand);
}

void BinaryExpressionParser::make_binary_expression()
{
    auto right = top_pop(operands);
    auto left = top_pop(operands);
    operands.push(std::make_shared<BinaryExpression>(left, right, top_pop(operators)));
}

std::shared_ptr<BinaryExpression> BinaryExpressionParser::parse()
{
    auto tokens = tokenizer.get_tokens();
    for (current_token = tokens.begin(); current_token != tokens.end(); ++current_token)
    {
        switch (current_token->type)
        {
        case TokenType::IDENTIFIER:
            read_identifier();
            break;
        case TokenType::INTEGER:
            operands.push(std::make_shared<ValueOperand>(std::stoi(current_token->value)));
            break;
        case TokenType::STRING:
            read_string(tokens.end());
            break;
        case TokenType::OPERATOR:
            read_operator();
            break;
        }
    }

    while (!operators.empty())
    {
        make_binary_expression();
    }

    if (operands.size()  != 1)
    {
        throw std::runtime_error("something's wrong");
    }

    return std::dynamic_pointer_cast<BinaryExpression>(operands.top());
}

}
