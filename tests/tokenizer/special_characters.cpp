
#include <gtest/gtest.h>
#include "frontend/tokenizer.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Tokenizing, Simple_Dot_Access) {
    std::string inline_input = "a.b";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].sourcetext, "a");
    EXPECT_EQ(tokens[1].sourcetext, ".");
    EXPECT_EQ(tokens[2].sourcetext, "b");
}

TEST(Tokenizing, Simple_Pointer_Dereference) {
    std::string inline_input = "#a";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].sourcetext, "#");
    EXPECT_EQ(tokens[1].sourcetext, "a");
}

TEST(Tokenizing, Simple_Mod_Operation) {
    std::string inline_input = "a%b";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].sourcetext, "a");
    EXPECT_EQ(tokens[1].sourcetext, "%");
    EXPECT_EQ(tokens[2].sourcetext, "b");
}