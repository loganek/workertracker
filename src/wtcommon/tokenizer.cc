/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "tokenizer.h"

#include <stdexcept>

namespace WT {

Tokenizer::Tokenizer(const std::string &text)
    : stream(text)
{
}

bool Tokenizer::move_next()
{
    stream.get();
    return stream.peek() != -1;
}

bool Tokenizer::is_operator_beginning()
{
    switch (peek())
    {
    case '=':
    case '~':
    case '>':
    case '<':
    case '|':
    case '&':
    case ')':
    case '(':
        return true;
    default:
        return false;
    }
}

void Tokenizer::reset()
{
    tokens.clear();
    stream.seekg(0);
}

char Tokenizer::peek()
{
    return static_cast<char>(stream.peek());
}

void Tokenizer::move_back()
{
    if (!stream.eof())
    {
        stream.putback(peek());
    }
}

void Tokenizer::read_integer()
{
    int value;
    stream >> value;
    move_back();
    tokens.emplace_back(TokenType::INTEGER, std::to_string(value));
}

void Tokenizer::read_identifier()
{
    std::string value;
    do
    {
        value += peek();
    } while (move_next() && isalnum(peek()));

    move_back();

    tokens.emplace_back(TokenType::IDENTIFIER, value);
}

void Tokenizer::read_string()
{
    std::string value;
    bool escaping = false;

    while (move_next() && (escaping || peek() != '"'))
    {
        if (!escaping)
        {
            if ((escaping = peek() == '\\')) continue;
        }
        else
        {
            escaping = false;

            if (peek() != '\\' && peek() != '"')
            {
                unexpected_token_character();
            }
        }

        value += peek();
    }

    if (peek() != '"')
    {
        unexpected_eof();
    }

    tokens.emplace_back(TokenType::STRING, value);
}

void Tokenizer::read_operator()
{
    std::string value(1, peek());
    char expected_next = 0;

    switch (peek())
    {
    case '<':
    case '>':
        expected_next = '=';
        break;
    }

    if (expected_next && move_next())
    {
        if (peek() == expected_next)
        {
            value += peek();
        }
        else
        {
            move_back();
        }
    }

    tokens.emplace_back(TokenType::OPERATOR, value);
}

std::vector<Token> Tokenizer::get_tokens()
{
    reset();

    if (stream.peek() == EOF)
    {
        return tokens;
    }

    do
    {
        if (isdigit(peek()))
        {
            read_integer();
        }
        else if (isalpha(peek()))
        {
            read_identifier();
        }
        else if (peek() == '"')
        {
            read_string();
        }
        else if (is_operator_beginning())
        {
            read_operator();
        }
        else if (!isspace(peek()))
        {
            unexpected_token_character();
        }
    } while (move_next());

    return tokens;
}

void Tokenizer::unexpected_token_character()
{
    throw TokenizerError(std::string("unexpected character '") + static_cast<char>(peek()) + "' on position " + std::to_string(stream.tellg()));
}

void Tokenizer::unexpected_eof()
{
    throw TokenizerError("unexpected EOF");
}

}
