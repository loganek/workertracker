#ifndef FILTEREXPRESSIONPARSER_H
#define FILTEREXPRESSIONPARSER_H

#include "filtertokenizer.h"

#include <boost/variant.hpp>

#include <map>

// todo replace with std::variant
typedef boost::variant<std::string, int, bool> result_t;

class BaseExpression
{
public:
    typedef std::shared_ptr<BaseExpression> Ptr;

    virtual ~BaseExpression() {}
    virtual result_t get_value() = 0;
    virtual bool get_bool_value() = 0;
};

template<typename T>
class LiteralExpression : public BaseExpression
{
    T value;

public:
    LiteralExpression(const T& value)
        : value(value)
    {}

    template<typename BT>
    bool get_bool(const BT& val)
    {
        return val;
    }

    bool get_bool(const std::string& val)
    {
        return !val.empty();
    }

    result_t get_value() override { return value; }

    bool get_bool_value() override { return get_bool(value); }
};

class BinaryExpression : public BaseExpression
{
private:
    BaseExpression::Ptr left;
    BaseExpression::Ptr right;
    char op;

public:
    BinaryExpression(BaseExpression::Ptr left, BaseExpression::Ptr right, char op);

    bool get_bool_value() override;
    result_t get_value() override;
};

class FilterExpressionParser
{
    std::vector<Token> tokens;
    std::size_t token_pos;
    std::map<std::string, BaseExpression::Ptr> variables;

    BaseExpression::Ptr read_expression();
    BaseExpression::Ptr read_literal_expression();

    char read_operator();

public:
    FilterExpressionParser(const std::string &text);

    template<typename T>
    void add_variable(const std::string& name, const T& value)
    {
        variables[name] = std::make_shared<LiteralExpression<T>>(value);
    }

    bool get_result();
};

#endif // FILTEREXPRESSIONPARSER_H
