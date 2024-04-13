
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Return_Nothing) {
    std::vector<Token> tokens = {
        { "return", "test.basalt", 1, 1, 1, Token::Type::text },
        { ";", "test.basalt", 1, 4, 4, Token::Type::symbol }
    };
    Parser parser = Parser(tokens);
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Return>());
    EXPECT_FALSE(statement.get<Return>().return_value.has_value());
}

TEST(Parsing, Return_Integer) {
    std::vector<Token> tokens = {
        { "return", "test.basalt", 1, 1, 1, Token::Type::text },
        { "6", "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { ";", "test.basalt", 1, 4, 4, Token::Type::symbol }
    };
    Parser parser = Parser(tokens);
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Return>());
    ASSERT_TRUE(statement.get<Return>().return_value.has_value());
    ASSERT_TRUE(statement.get<Return>().return_value->is<IntLiteral>());
    EXPECT_EQ(statement.get<Return>().return_value->get<IntLiteral>().value, 6);
}

TEST(Parsing, Return_Expression) {
    std::vector<Token> tokens = {
        { "return", "test.basalt", 1, 1, 1, Token::Type::text },
        { "6", "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { "*", "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { "7", "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { ";", "test.basalt", 1, 4, 4, Token::Type::symbol }
    };
    Parser parser = Parser(tokens);
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Return>());
    ASSERT_TRUE(statement.get<Return>().return_value.has_value());
    ASSERT_TRUE(statement.get<Return>().return_value->is<BinaryOperator>());
}