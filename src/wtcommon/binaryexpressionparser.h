#ifndef BINARYEXPRESSIONPARSER_H
#define BINARYEXPRESSIONPARSER_H

#include "binaryexpressiontree.h"

#include <unordered_map>
#include <stack>

namespace WT {

class BinaryExpressionParser
{
    std::unordered_map<std::string, operand_value_t> variables;

    std::stack<Operator> operators;
    std::stack<operand_ptr_t> operands;

    std::string expression_str;
    std::size_t pos;

    void reset() { pos = -1; }

    bool is_eof();
    bool move_next();
    char peek();
    void back();

    void read_identifier();
    void read_number();
    void read_string();
    void read_operator();

    bool is_operator_beginning();

    std::exception unexpected_eof();
    std::exception unexpected_character();

    void make_binary_expression();

    static std::shared_ptr<ValueOperand> get_datetime_from_string(const std::string& str);

    template<typename T>
    static T top_pop(std::stack<T>& stack)
    {
        if (stack.empty())
            throw std::runtime_error("unexpected empty stack!");

        auto val = stack.top();
        stack.pop();
        return val;
    }

public:
    BinaryExpressionParser(const std::string &expression_str, const std::unordered_map<std::string, operand_value_t>& variables = {});

    std::shared_ptr<BinaryExpression> parse();
};

}

#endif // BINARYEXPRESSIONPARSER_H
