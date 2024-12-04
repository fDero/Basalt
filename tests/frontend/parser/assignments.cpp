
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Frontend, Parse_Trivial_Assignment) {
    std::vector<Token> tokens = {
        { "x", "test.basalt", 1, 1, 1, Token::Type::text },
        { "=", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "6", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
        { ";", "test.basalt", 1, 4, 4, Token::Type::symbol }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Assignment>());
    EXPECT_TRUE(statement.get<Assignment>().assigned_value.is<IntLiteral>());
    EXPECT_TRUE(statement.get<Assignment>().assignment_target.is<Identifier>());
}

TEST(Frontend, Parse_Assignment_Of_Expression_To_Pointer_Dereference) {
    std::vector<Token> tokens = {
        { "#", "test.basalt", 1, 1, 1, Token::Type::symbol },
        { "x", "test.basalt", 1, 2, 2, Token::Type::text },
        { "=", "test.basalt", 1, 3, 3, Token::Type::symbol },
        { "6", "test.basalt", 1, 4, 4, Token::Type::integer_literal },
        { "*", "test.basalt", 1, 5, 5, Token::Type::symbol },
        { "2", "test.basalt", 1, 6, 6, Token::Type::integer_literal },
        { ";", "test.basalt", 1, 7, 7, Token::Type::symbol }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_statement();
    ASSERT_TRUE(statement.is<Assignment>());
    EXPECT_TRUE(statement.get<Assignment>().assigned_value.is<BinaryOperator>());
    EXPECT_TRUE(statement.get<Assignment>().assignment_target.is<UnaryOperator>());
}