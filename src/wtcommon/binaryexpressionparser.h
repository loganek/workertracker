#ifndef BINARYEXPRESSIONPARSER_H
#define BINARYEXPRESSIONPARSER_H

#include "binaryexpressiontree.h"
#include <stack>

namespace WT {

class BinaryExpressionParser
{
    std::stack<char> operators;
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

    bool is_operator(char c);

    std::exception unexpected_eof();
    std::exception unexpected_character();

    void make_binary_expression();

public:
    BinaryExpressionParser(const std::string &expression_str);

    std::shared_ptr<BinaryExpression> parse();
};

}

#endif // BINARYEXPRESSIONPARSER_H