#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "frontend/token.hpp"
#include "frontend/tokenized_file.hpp"
#include "frontend/tokenizer.hpp"


TEST(Frontend, Tokenize_Simple_Dot_Access) {
    std::string inline_input = "a.b";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].sourcetext, "a");
    EXPECT_EQ(tokens[1].sourcetext, ".");
    EXPECT_EQ(tokens[2].sourcetext, "b");
}

TEST(Frontend, Tokenize_Simple_Pointer_Dereference) {
    std::string inline_input = "#a";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].sourcetext, "#");
    EXPECT_EQ(tokens[1].sourcetext, "a");
}

TEST(Frontend, Tokenize_Simple_Mod_Operation) {
    std::string inline_input = "a%b";
    Tokenizer tokenizer = Tokenizer(std::istringstream(inline_input));
    std::vector<Token> tokens = tokenizer.tokenize().tokens;
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].sourcetext, "a");
    EXPECT_EQ(tokens[1].sourcetext, "%");
    EXPECT_EQ(tokens[2].sourcetext, "b");
}