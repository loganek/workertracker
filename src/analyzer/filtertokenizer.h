#ifndef FILTERTOKENIZER_H
#define FILTERTOKENIZER_H

#include <string>
#include <vector>

enum class TokenType
{
    NONE,
    STRING,
    INTEGER,
    IDENTIFIER,
    OPERATOR
};

struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType type, std::string value)
        : type(type), value(value)
    {}
};

class FilterTokenizer
{
    std::string text;
    std::vector<Token> tokens;
    TokenType cur_token_type;
    std::string cur_token_val;
    std::size_t pos;

    void insert_token();

    void unexpected_token_character(char c);

public:
    FilterTokenizer(const std::string &text);

    std::vector<Token> get_tokens();
};

#endif // FILTERTOKENIZER_H
