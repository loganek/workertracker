#ifndef BINARYEXPRESSIONTREE_H
#define BINARYEXPRESSIONTREE_H

#include <memory>
#include <regex>
#include <ctime>

#include <boost/variant.hpp>

namespace WT {

typedef  boost::variant<std::int64_t, std::string, bool, std::tm> operand_value_t;

class Operand
{
public:
    virtual ~Operand() {}

    virtual operand_value_t get_value() = 0;
};

using operand_ptr_t = std::shared_ptr<Operand>;

class ValueOperand : public Operand
{
    operand_value_t value;

public:
    ValueOperand(const operand_value_t& value) : value(value) {}
    operand_value_t get_value() override { return value; }
};

class VariableOperand : public Operand
{
    std::string name;
    operand_value_t value;

public:
    VariableOperand(const std::string &name, const operand_value_t& value) : name(name), value(value) {}
    operand_value_t get_value() override { return value; }
    std::string get_name() const { return name; }
};

class BinaryExpression : public Operand
{
    char op;
    std::shared_ptr<Operand> left;
    std::shared_ptr<Operand> right;

    bool get_operand_value(const operand_value_t& left, const operand_value_t& right, char op);

public:
    BinaryExpression(const operand_ptr_t& left, const operand_ptr_t& right, char op)
        : op(op), left(left), right(right)
    {}

    char get_operator() const { return op; }
    operand_ptr_t get_left_operand() const { return left; }
    operand_ptr_t get_right_operand() const { return right; }

    operand_value_t get_value() override
    {
        return get_operand_value(left->get_value(), right->get_value(), op);
    }

    bool get_result() { return boost::get<bool>(get_value()); }

    static int get_operator_precedence(char op);
};

}

#endif // BINARYEXPRESSIONTREE_H
