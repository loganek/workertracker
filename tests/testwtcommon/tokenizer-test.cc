/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "gtest/gtest.h"

#include "wtcommon/tokenizer.h"

using namespace WT;

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
    std::vector<Token> tokens = Tokenizer("marcin=12<6").get_tokens();

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
    auto tokens = Tokenizer("32 test&2 33>=marcin6=2").get_tokens();

    std::vector<Token> expected_tokens = {
        Token(TokenType::INTEGER, "32"),
        Token(TokenType::IDENTIFIER, "test"),
        Token(TokenType::OPERATOR, "&"),
        Token(TokenType::INTEGER, "2"),
        Token(TokenType::INTEGER, "33"),
        Token(TokenType::OPERATOR, ">="),
        Token(TokenType::IDENTIFIER, "marcin6"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::INTEGER, "2")
    };

    assert_tokens(expected_tokens, tokens);
}


TEST(FilterTokenizerTest, InvalidCharacters_ShouldThrow)
{
    EXPECT_THROW(Tokenizer("42.32").get_tokens(), TokenizerError);
}

TEST(FilterTokenizerTest, StringToken)
{
    auto tokens = Tokenizer("marcin=\"test with numbers (33), spaces, and invalid characters (like '.')\"").get_tokens();

    std::vector<Token> expected_tokens = {
        Token(TokenType::IDENTIFIER, "marcin"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::STRING, "test with numbers (33), spaces, and invalid characters (like '.')")
    };

    assert_tokens(expected_tokens, tokens);
}

TEST(FilterTokenizerTest, StringToken_EmptyString)
{
    auto tokens = Tokenizer("marcin=\"\"").get_tokens();

    std::vector<Token> expected_tokens = {
        Token(TokenType::IDENTIFIER, "marcin"),
        Token(TokenType::OPERATOR, "="),
        Token(TokenType::STRING, "")
    };

    assert_tokens(expected_tokens, tokens);
}

TEST(FilterTokenizerTest, StringToken_MissingClosingQuote_ShouldThrow)
{
    EXPECT_THROW(Tokenizer("marcin=\"test").get_tokens(), TokenizerError);
}

TEST(FilterTokenizerTest, EmptyInputString_TokensVectorShouldBeEmpty)
{
    ASSERT_EQ(std::size_t(0), Tokenizer("").get_tokens().size());
}

TEST(FilterTokenizerTest, EscapeString)
{
    auto tokens = Tokenizer("\"this \\\\ is \\\"escaped\\\" string\"").get_tokens();

    std::vector<Token> expected_tokens = {
        Token(TokenType::STRING, "this \\ is \"escaped\" string")
    };

    assert_tokens(expected_tokens, tokens);
}

TEST(FilterTokenizerTest, EscapedString_InvalidCharacter_ShouldThrow)
{
    EXPECT_THROW(Tokenizer("\"this is \\\"escaped\\c string\"").get_tokens(), TokenizerError);
}


TEST(FilterTokenizerTest, EscapedString_EscapedLastQuotation_ShouldThrow)
{
    EXPECT_THROW(Tokenizer("\"test \\\"").get_tokens(), TokenizerError);
}

