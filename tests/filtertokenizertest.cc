#include "analyzer/filtertokenizer.h"

#include <gtest/gtest.h>

static void assert_tokens(const std::vector<Token>& expected_tokens, const std::vector<Token>& actual_tokens)
{
    ASSERT_EQ(expected_tokens.size(), actual_tokens.size());

    for (std::size_t i = 0; i < actual_tokens.size(); i++)
    {
        ASSERT_EQ(expected_tokens[i].type, actual_tokens[i].type);
        ASSERT_EQ(expected_tokens[i].value, actual_tokens[i].value);
    }
}

TEST(FilterTokenizerTest, SimpleTokenizeWithoutSpaces)
{
    auto tokens = FilterTokenizer("marcin=12<6").get_tokens();

    std::vector<Token> expected_tokens = {
        Token(TokenType::IDENTIFIER, "marcin"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::INTEGER, "12"),
        Token(TokenType::OPERATOR, "<"),
        Token(TokenType::INTEGER, "6")
    };

    assert_tokens(expected_tokens, tokens);
}

TEST(FilterTokenizerTest, SimpleTokenizeWithSpaces)
{
    auto tokens = FilterTokenizer("32 test&2 33>marcin6=2").get_tokens();

    std::vector<Token> expected_tokens = {
        Token(TokenType::INTEGER, "32"),
        Token(TokenType::IDENTIFIER, "test"),
        Token(TokenType::OPERATOR, "&"),
        Token(TokenType::INTEGER, "2"),
        Token(TokenType::INTEGER, "33"),
        Token(TokenType::OPERATOR, ">"),
        Token(TokenType::IDENTIFIER, "marcin6"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::INTEGER, "2")
    };

    assert_tokens(expected_tokens, tokens);
}


TEST(FilterTokenizerTest, InvalidCharacters_ShouldThrow)
{
    EXPECT_THROW(FilterTokenizer("42.32").get_tokens(), std::runtime_error);
}

TEST(FilterTokenizerTest, StringToken)
{
    auto tokens = FilterTokenizer("marcin=\"test with numbers (33), spaces, and invalid characters (like '.')\"").get_tokens();

    std::vector<Token> expected_tokens = {
        Token(TokenType::IDENTIFIER, "marcin"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::STRING, "test with numbers (33), spaces, and invalid characters (like '.')")
    };

    assert_tokens(expected_tokens, tokens);
}

TEST(FilterTokenizerTest, StringToken_EmptyString)
{
    auto tokens = FilterTokenizer("marcin=\"\"").get_tokens();

    std::vector<Token> expected_tokens = {
        Token(TokenType::IDENTIFIER, "marcin"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::STRING, "")
    };

    assert_tokens(expected_tokens, tokens);
}

TEST(FilterTokenizerTest, StringToken_MissingClosingQuote)
{
    EXPECT_THROW(FilterTokenizer("marcin=\"test").get_tokens(), std::runtime_error);
}

TEST(FilterTokenizerTest, StringToken_InvalidIntegerLiteral)
{
    EXPECT_THROW(FilterTokenizer("123a").get_tokens(), std::runtime_error);
}
