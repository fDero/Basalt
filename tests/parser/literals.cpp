
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Integer_Literals) {
    std::vector<Token> tokens = {
        { "123",  "test.basalt",  1, 1, 1,  Token::Type::integer_literal },
        { "0",    "test.basalt",  1, 2, 4,  Token::Type::integer_literal },
        { "500",  "test.basalt",  1, 3, 5,  Token::Type::integer_literal }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    for (const auto& literal : tokens) {
        Expression expr = parser.parse_integer_literal();
        ASSERT_TRUE(expr.is<IntLiteral>());
        EXPECT_EQ(expr.get<IntLiteral>().value, std::stoi(literal.sourcetext));
    }
}

TEST(Parsing, String_Literals) {
    std::vector<Token> tokens = {
        { "\"Hello\"",  "test.basalt",  1, 1, 1,  Token::Type::string_literal },
        { "\"World\"",  "test.basalt",  1, 2, 8,  Token::Type::string_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    for (const auto& literal : tokens) {
        Expression expr = parser.parse_string_literal();
        ASSERT_TRUE(expr.is<StringLiteral>());
        EXPECT_EQ(expr.get<StringLiteral>().value, literal.sourcetext);
    }
}

TEST(Parsing, Floating_Literals) {
    std::vector<Token> tokens = {
        { "123.0",  "test.basalt",  1, 1, 1,  Token::Type::floating_literal },
        { "0.0",    "test.basalt",  1, 2, 4,  Token::Type::floating_literal },
        { "5.002",  "test.basalt",  1, 3, 5,  Token::Type::floating_literal }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    for (const auto& literal : tokens) {
        Expression expr = parser.parse_floating_literal();
        ASSERT_TRUE(expr.is<FloatLiteral>());
        EXPECT_EQ(expr.get<FloatLiteral>().value, std::stod(literal.sourcetext));
    }
}

TEST(Parsing, Char_Literals) {
    std::vector<Token> tokens = {
        { "'x'",   "test.basalt",  1, 1, 1,  Token::Type::character_literal },
        { "'y'",   "test.basalt",  1, 2, 2,  Token::Type::character_literal },
        { "'z'",   "test.basalt",  1, 3, 3,  Token::Type::character_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    for (const auto& literal : tokens) {
        Expression expr = parser.parse_character_literal();
        ASSERT_TRUE(expr.is<CharLiteral>());
        EXPECT_EQ(expr.get<CharLiteral>().value, literal.sourcetext[1]);
    }
}

TEST(Parsing, Boolean_Literals) {
    std::vector<Token> tokens = {
        { "true",   "test.basalt",  1, 1, 1,  Token::Type::boolean_literal },
        { "false",  "test.basalt",  1, 2, 5,  Token::Type::boolean_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    for (const auto& literal : tokens) {
        Expression expr = parser.parse_boolean_literal();
        ASSERT_TRUE(expr.is<BoolLiteral>());
        EXPECT_EQ(expr.get<BoolLiteral>().value, (literal.sourcetext == "true")? true : false);
    }
}