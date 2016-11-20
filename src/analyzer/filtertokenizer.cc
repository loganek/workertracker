#include "filtertokenizer.h"

#include <stdexcept>

FilterTokenizer::FilterTokenizer(const std::string &text)
    : text(text)
{
}

static bool is_operator(char c)
{
    switch (c)
    {
    case '=':
    case '~':
    case '>':
    case '<':
    case '|':
    case '&':
        return true;
    default:
        return false;
    }
}

void FilterTokenizer::insert_token()
{
    if (cur_token_type == TokenType::NONE)
    {
        return;
    }

    tokens.emplace_back(cur_token_type, cur_token_val);
    cur_token_type = TokenType::NONE;
    pos--;
}

std::vector<Token> FilterTokenizer::get_tokens()
{
    tokens.clear();
    cur_token_type = TokenType::NONE;

    for (pos = 0; pos < text.size(); pos++)
    {
        char c = text[pos];

        switch (cur_token_type)
        {
        case TokenType::NONE:
            cur_token_val = "";
            if (isalpha(c)) cur_token_type = TokenType::IDENTIFIER;
            else if (c == '"')
            {
                cur_token_type = TokenType::STRING;
                continue;
            }
            else if (isdigit(c)) cur_token_type = TokenType::INTEGER;
            else if (is_operator(c)) cur_token_type = TokenType::OPERATOR;
            else if (!isspace(c)) unexpected_token_character(c);
            break;

        case TokenType::IDENTIFIER:
            if (isspace(c) || is_operator(c))
            {
                insert_token();
            }
            else if (!isalnum(c))
            {
                unexpected_token_character(c);
            }
            break;

        case TokenType::INTEGER:
            if (isspace(c) || is_operator(c))
            {
                insert_token();
            }
            else if (!isdigit(c))
            {
                unexpected_token_character(c);
            }
            break;
        case TokenType::STRING:
            if (c == '"') // TODO escape scring
            {
                insert_token();
                pos++;
            }
            break;
        case TokenType::OPERATOR:
            insert_token();
            break;
        }

        if (cur_token_type != TokenType::NONE)
        {
            cur_token_val += c;
        }
    }

    if (cur_token_type == TokenType::STRING)
    {
        throw std::runtime_error("expected '\"' at the end of the string");
    }
    else
    {
        insert_token();
    }

    return tokens;
}

void FilterTokenizer::unexpected_token_character(char c)
{
    throw std::runtime_error(std::string("unexpected character '") + c + "' on position " + std::to_string(pos));
}
