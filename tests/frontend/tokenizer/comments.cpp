
#include <gtest/gtest.h>
#include "frontend/tokenizer.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Frontend, Tokenize_Nested_Multiline_Comments_compact) {
    std::string inline_input = "16/*/*comment*/*/17";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].sourcetext, "16");
    EXPECT_EQ(tokens[1].sourcetext, "17");
    EXPECT_EQ(tokens[0].type, Token::Type::integer_literal);
    EXPECT_EQ(tokens[1].type, Token::Type::integer_literal);
}

TEST(Frontend, Tokenize_Nested_Multiline_Comments_hyper_spread) {
    std::string inline_input = " 16   /*   /*   comment   */  */  17";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].sourcetext, "16");
    EXPECT_EQ(tokens[1].sourcetext, "17");
    EXPECT_EQ(tokens[0].type, Token::Type::integer_literal);
    EXPECT_EQ(tokens[1].type, Token::Type::integer_literal);
}

TEST(Frontend, Tokenize_Nested_Multiline_Comments_full_spread) {
    std::string inline_input = "16 /* /* comment */ */ 17";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].sourcetext, "16");
    EXPECT_EQ(tokens[1].sourcetext, "17");
    EXPECT_EQ(tokens[0].type, Token::Type::integer_literal);
    EXPECT_EQ(tokens[1].type, Token::Type::integer_literal);
}

TEST(Frontend, Tokenize_Nested_Multiline_Comments_half_spread) {
    std::string inline_input = "16 /*/* comment*/ */17";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].sourcetext, "16");
    EXPECT_EQ(tokens[1].sourcetext, "17");
    EXPECT_EQ(tokens[0].type, Token::Type::integer_literal);
    EXPECT_EQ(tokens[1].type, Token::Type::integer_literal);
}

TEST(Frontend, Tokenize_Nested_Multiline_Comments_on_two_lines) {
    std::string inline_input = "16 /*/* \n \tcomment \n */ */17";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].sourcetext, "16");
    EXPECT_EQ(tokens[1].sourcetext, "17");
    EXPECT_EQ(tokens[0].type, Token::Type::integer_literal);
    EXPECT_EQ(tokens[1].type, Token::Type::integer_literal);
}

TEST(Frontend, Tokenize_Single_Line_Comment) {
    std::string inline_input = "16 // comment  */ */17";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].sourcetext, "16");
    EXPECT_EQ(tokens[0].type, Token::Type::integer_literal);
}