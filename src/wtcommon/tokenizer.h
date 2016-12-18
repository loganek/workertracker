/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <sstream>
#include <vector>

namespace WT {

enum class TokenType
{
    STRING,
    INTEGER,
    IDENTIFIER,
    OPERATOR
};

struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string& value)
        : type(type), value(value)
    {}
};

class TokenizerError : public std::runtime_error
{
public:
    TokenizerError(const std::string &msg)
        : std::runtime_error(msg)
    {}
};

class Tokenizer
{
    std::stringstream stream;
    std::vector<Token> tokens;

    void insert_token();

    void unexpected_token_character();
    void unexpected_eof();

    void reset();
    bool move_next();
    void move_back();
    char peek();

    bool is_operator_beginning();

    void read_integer();
    void read_identifier();
    void read_string();
    void read_operator();

public:
    Tokenizer(const std::string &text);

    std::vector<Token> get_tokens();
};

}

#endif // TOKENIZER_H
