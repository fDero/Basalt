
#include <gtest/gtest.h>
#include "toolchain/tokenizer.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Tokenizing, Complex_Math_Expression) {
    std::string inline_input = "(16+17*19)/2;";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 10);
    EXPECT_EQ(tokens[0].sourcetext, "(");
    EXPECT_EQ(tokens[1].sourcetext, "16");
    EXPECT_EQ(tokens[2].sourcetext, "+");
    EXPECT_EQ(tokens[3].sourcetext, "17");
    EXPECT_EQ(tokens[4].sourcetext, "*");
    EXPECT_EQ(tokens[5].sourcetext, "19");
    EXPECT_EQ(tokens[6].sourcetext, ")");
    EXPECT_EQ(tokens[7].sourcetext, "/");
    EXPECT_EQ(tokens[8].sourcetext, "2");
    EXPECT_EQ(tokens[9].sourcetext, ";");
}

TEST(Tokenizing, Complex_Logical_Expression) {
    std::string inline_input = "!(a && b || !c);";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 10);
    EXPECT_EQ(tokens[0].sourcetext, "!");
    EXPECT_EQ(tokens[1].sourcetext, "(");
    EXPECT_EQ(tokens[2].sourcetext, "a");
    EXPECT_EQ(tokens[3].sourcetext, "&&");
    EXPECT_EQ(tokens[4].sourcetext, "b");
    EXPECT_EQ(tokens[5].sourcetext, "||");
    EXPECT_EQ(tokens[6].sourcetext, "!");
    EXPECT_EQ(tokens[7].sourcetext, "c");
    EXPECT_EQ(tokens[8].sourcetext, ")");
    EXPECT_EQ(tokens[9].sourcetext, ";");
}

TEST(Tokenizing, Expression_With_Comments) {
    std::string inline_input = "/*/**/*/(16+17* /*AA*/ 19)/2; // hello world";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize();
    ASSERT_EQ(tokens.size(), 10);
    EXPECT_EQ(tokens[0].sourcetext, "(");
    EXPECT_EQ(tokens[1].sourcetext, "16");
    EXPECT_EQ(tokens[2].sourcetext, "+");
    EXPECT_EQ(tokens[3].sourcetext, "17");
    EXPECT_EQ(tokens[4].sourcetext, "*");
    EXPECT_EQ(tokens[5].sourcetext, "19");
    EXPECT_EQ(tokens[6].sourcetext, ")");
    EXPECT_EQ(tokens[7].sourcetext, "/");
    EXPECT_EQ(tokens[8].sourcetext, "2");
    EXPECT_EQ(tokens[9].sourcetext, ";");
}