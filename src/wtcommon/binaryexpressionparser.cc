#include "binaryexpressionparser.h"

#include <iomanip>

namespace WT {

static int precedence(char c)
{
    return BinaryExpression::get_operator_precedence(c);
}

BinaryExpressionParser::BinaryExpressionParser(const std::string &expression_str)
    : expression_str(expression_str)
{

}

bool BinaryExpressionParser::is_eof()
{
    return pos >= expression_str.size();
}

bool BinaryExpressionParser::move_next()
{
    pos++;
    return !is_eof();
}

void BinaryExpressionParser::back()
{
    pos--;
}

char BinaryExpressionParser::peek()
{
    return expression_str.at(pos);
}

bool BinaryExpressionParser::is_operator(char c)
{
    return precedence(c) > 0;
}

void BinaryExpressionParser::read_identifier()
{
    std::string identifier;
    do {
        identifier += peek();
    } while (move_next() && isalpha(peek()));

    if (!is_eof())
    {
        if (peek() != ')' && !is_operator(peek()) && !isspace(peek()))
            throw unexpected_character();
        back();
    }

    operands.push(std::make_shared<VariableOperand>(identifier));
}

void BinaryExpressionParser::read_number()
{
    std::string number;
    do
    {
        number += peek();
    } while (move_next() && isdigit(peek()));

    if (!is_eof())
    {
        if (peek() != ')' && !is_operator(peek()) && !isspace(peek()))
            throw unexpected_character();
        back();
    }

    operands.push(std::make_shared<ValueOperand>(static_cast<std::int64_t>(std::stoll(number))));
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

void BinaryExpressionParser::read_string()
{
    std::string value;
    while (move_next() && peek() != '"') // TODO escape string
    {
        value += peek();
    }

    if (is_eof())
        throw unexpected_eof();

    move_next();

    std::shared_ptr<ValueOperand> operand;

    if (!is_eof())
    {
        if (peek() == 'd')
            operand = get_datetime_from_string(value);
        else if (peek() != ')' && !is_operator(peek()) && !isspace(peek()))
            throw unexpected_character();
        else
            back();
    }

    if (!operand) operand = std::make_shared<ValueOperand>(value);

    operands.push(operand);
}

void BinaryExpressionParser::read_operator()
{
    char op = peek();

    if (!move_next())
        throw unexpected_eof();
    else if (is_operator(peek()))
        throw unexpected_character();

    back();

    int op_precendence = precedence(op);

    while ((!operators.empty()) && (op_precendence <= precedence(operators.top())))
    {
        make_binary_expression();
    }

    operators.push(op);
}

void BinaryExpressionParser::make_binary_expression()
{
    auto right = top_pop(operands);
    auto left = top_pop(operands);
    operands.push(std::make_shared<BinaryExpression>(left, right, top_pop(operators)));
}

std::shared_ptr<BinaryExpression> BinaryExpressionParser::parse()
{
    reset();

    while (move_next())
    {
        if (isspace(peek())) continue;

        if (isalpha(peek())) { read_identifier(); }
        else if (isdigit(peek())) { read_number(); }
        else if (is_operator(peek())) { read_operator(); }
        else if (peek() == '"') { read_string(); }
        else if (peek() == '(') operators.push(peek());
        else if (peek() == ')')
        {
            while (!operators.empty() && operators.top() != '(')
            {
                make_binary_expression();
            }
            operators.pop();
        }
    }

    while (!operators.empty())
    {
        make_binary_expression();
    }

    if (operands.size()  != 1)
    {
        throw std::runtime_error("somethings wrong");
    }

    return std::dynamic_pointer_cast<BinaryExpression>(operands.top());
}

std::exception BinaryExpressionParser::unexpected_eof()
{
    return std::runtime_error("Unexpected EoF!");
}


std::exception BinaryExpressionParser::unexpected_character()
{
    return std::runtime_error(std::string("Unexpected character '") + peek() + "' on position " + std::to_string(pos));
}

}
