/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "binaryexpressiontree.h"

#include <map>
#include <functional>

// It's not a singleton, however, I'd like to easy way to get single instance across different places
#define DECLARE_TYPE(TYPE) \
    template<> \
    class OperatorFunctions<TYPE> : public OperatorFunctionsBase<TYPE>\
    { \
        static OperatorFunctions instance; \
        void init_operators(); \
    public: \
        OperatorFunctions() { \
            init_operators(); \
        } \
        static OperatorFunctions& get() { return instance; } \
    }; \
    OperatorFunctions<TYPE> OperatorFunctions<TYPE>::instance;

#define DEFINE_OPERATOR(OP_VALUE, OP) \
    functions[OP_VALUE] = [](const this_type_t& left, const this_type_t& right) { return left OP right; };

namespace WT {

class ValueFunctionsInitializer
{
public:
    static std::map<std::size_t, std::function<bool(const operand_value_t&, const operand_value_t&, Operator)>> value_functions;
};
std::map<std::size_t, std::function<bool(const operand_value_t&, const operand_value_t&, Operator)>> ValueFunctionsInitializer::value_functions;

template<typename T>
class OperatorFunctions;

template<typename T>
class OperatorFunctionsBase
{
protected:
    typedef T this_type_t;
    std::map<Operator, std::function<bool(const T&, const T&)>> functions;

public:
    OperatorFunctionsBase()
    {
        ValueFunctionsInitializer::value_functions[typeid(T).hash_code()] =
                [] (const operand_value_t& left, const operand_value_t& right, Operator op) {
                    auto& of = OperatorFunctions<T>::get();
                    if (of.implements_operator(op))
                    {
                        return of.call(boost::get<T>(left), boost::get<T>(right), op);
                    }

                    throw std::runtime_error("invalid operator \"" + std::to_string((int)op) + "\" for type " + std::string(typeid(T).name()));
                };
    }

    bool implements_operator(Operator op) { return functions.find(op) != functions.end(); }
    bool call(const T& left, const T& right, Operator op) { return functions.at(op)(left, right); }
};

bool BinaryExpression::get_operand_value(const operand_value_t& left, const operand_value_t& right, Operator op)
{
    if (left.type() != right.type())
    {
        throw std::runtime_error("types don't matach, left: " + std::string(left.type().name()) + ", right: " + right.type().name());
    }

    auto it = ValueFunctionsInitializer::value_functions.find(left.type().hash_code());
    if (it != ValueFunctionsInitializer::value_functions.end())
        return it->second(left, right, op);
    throw std::runtime_error("unknown type " + std::string(left.type().name()));
}

DECLARE_TYPE(std::string)
DECLARE_TYPE(int)
DECLARE_TYPE(std::tm)
DECLARE_TYPE(bool)

void OperatorFunctions<std::string>::init_operators()
{
    DEFINE_OPERATOR(Operator::EQ, ==)
    DEFINE_OPERATOR(Operator::GT, >)
    DEFINE_OPERATOR(Operator::GT, >=)
    DEFINE_OPERATOR(Operator::LT, <)
    DEFINE_OPERATOR(Operator::LT, <=)
    DEFINE_OPERATOR(Operator::NEQ, !=)
    functions[Operator::MATCH] = [](const this_type_t& left, const this_type_t& right) { return std::regex_search(left, std::regex(right)); };
}

void OperatorFunctions<int>::init_operators()
{
    DEFINE_OPERATOR(Operator::EQ, ==)
    DEFINE_OPERATOR(Operator::GT, >)
    DEFINE_OPERATOR(Operator::GE, >=)
    DEFINE_OPERATOR(Operator::LT, <)
    DEFINE_OPERATOR(Operator::LE, <=)
    DEFINE_OPERATOR(Operator::NEQ, !=)
}

#define TM_OPERATOR(OP) bool operator OP(const std::tm& t1, const std::tm& t2) { return std::mktime(const_cast<std::tm*>(&t1)) OP std::mktime(const_cast<std::tm*>(&t2)); }
TM_OPERATOR(==)
TM_OPERATOR(!=)
TM_OPERATOR(>)
TM_OPERATOR(>=)
TM_OPERATOR(<)
TM_OPERATOR(<=)

void OperatorFunctions<std::tm>::init_operators()
{
    DEFINE_OPERATOR(Operator::EQ, ==)
    DEFINE_OPERATOR(Operator::GT, >)
    DEFINE_OPERATOR(Operator::GT, >=)
    DEFINE_OPERATOR(Operator::LT, <)
    DEFINE_OPERATOR(Operator::LT, <=)
    DEFINE_OPERATOR(Operator::NEQ, !=)
}

void OperatorFunctions<bool>::init_operators()
{
    DEFINE_OPERATOR(Operator::AND, &&)
    DEFINE_OPERATOR(Operator::OR, ||)
}


int BinaryExpression::get_operator_precedence(Operator op)
{
    if (OperatorFunctions<std::string>::get().implements_operator(op)
        || OperatorFunctions<std::tm>::get().implements_operator(op)
        || OperatorFunctions<int>::get().implements_operator(op))
    {
        return 2;
    }
    if (OperatorFunctions<bool>::get().implements_operator(op))
    {
        return 1;
    }
    return 0;
}

}
