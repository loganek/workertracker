/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef BINARYEXPRESSIONPARSER_H
#define BINARYEXPRESSIONPARSER_H

#include "binaryexpressiontree.h"
#include "tokenizer.h"

#include <unordered_map>
#include <stack>

namespace WT {

class BinaryExpressionParser
{
    std::unordered_map<std::string, operand_value_t> variables;

    std::stack<Operator> operators;
    std::stack<operand_ptr_t> operands;

    Tokenizer tokenizer;
    std::vector<Token>::iterator current_token;

    void read_operator();
    void read_identifier();
    void read_string(const std::vector<Token>::iterator& last);

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
