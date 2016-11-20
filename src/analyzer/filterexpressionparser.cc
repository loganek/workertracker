#include "filterexpressionparser.h"

bool BinaryExpression::get_bool_value()
{
    return boost::get<bool>(get_value());
}

result_t BinaryExpression::get_value()
{
#define CASE_FOR_OPERATOR(T_OP, CPP_OP) \
    case T_OP: \
        return left->get_value() CPP_OP right->get_value();

    switch (op)
    {
    CASE_FOR_OPERATOR('=', ==)
    CASE_FOR_OPERATOR('<', <)
    CASE_FOR_OPERATOR('>', >)
    case '&':
        return left->get_bool_value() && right->get_bool_value();
    default:
        throw std::runtime_error("unknown");
    }
}

BinaryExpression::BinaryExpression(BaseExpression::Ptr left, BaseExpression::Ptr right, char op)
    : left(left), right(right), op(op)
{
    if (left->get_value().type() != right->get_value().type())
        throw std::runtime_error("cannot compare different types");
}

FilterExpressionParser::FilterExpressionParser(const std::string &text)
{
    tokens = FilterTokenizer(text).get_tokens();
}

bool FilterExpressionParser::get_result()
{
    BaseExpression::Ptr expr = nullptr;

    for (token_pos = 0; token_pos < tokens.size();)
    {
        if (expr == nullptr)
            expr = read_expression();
        else
        {
            auto op = read_operator();
            auto right = read_expression();
            expr = std::make_shared<BinaryExpression>(expr, right, op);
        }
    }

    return expr->get_bool_value();
}

BaseExpression::Ptr FilterExpressionParser::read_expression()
{
    BaseExpression::Ptr left = read_literal_expression();

    if (tokens.size() == token_pos)
        return left;

    char op = read_operator();
    BaseExpression::Ptr right = read_literal_expression();

    return std::make_shared<BinaryExpression>(left, right, op);
}

BaseExpression::Ptr FilterExpressionParser::read_literal_expression()
{
    auto& token = tokens[token_pos++];
    switch (token.type)
    {
    case TokenType::IDENTIFIER:
    {
        auto it = variables.find(token.value);
        if (it == variables.end())
        {
            throw std::runtime_error("variable does not exist");
        }
        return it->second;
    }
    case TokenType::STRING:
        return std::make_shared<LiteralExpression<std::string>>(token.value);
    case TokenType::INTEGER:
        return std::make_shared<LiteralExpression<int>>(std::stoi(token.value));
    default:
        throw std::runtime_error("cannot create expression from this token");
    }
}

char FilterExpressionParser::read_operator()
{
    if (tokens[token_pos].type != TokenType::OPERATOR)
    {
        throw std::runtime_error("it's not an operator!");
    }

    return tokens[token_pos++].value.front();
}
