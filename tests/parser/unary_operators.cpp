
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Simple_Unary_Operator) {
    std::vector<Token> tokens = {
        { "-", "test.basalt", 1, 1, 1, Token::Type::symbol },
        { "9", "test.basalt", 1, 2, 2, Token::Type::integer_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<UnaryOperator>());
    EXPECT_TRUE(expr.get<UnaryOperator>().operand.is<IntLiteral>());
    EXPECT_EQ(expr.get<UnaryOperator>().operator_text, "-");
    EXPECT_EQ(expr.get<UnaryOperator>().operand.get<IntLiteral>().value, 9);
}

TEST(Parsing, Nested_Unary_Operators) {
    std::vector<Token> tokens = {
        { "-", "test.basalt", 1, 1, 1, Token::Type::symbol },
        { "-", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "9", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.get<UnaryOperator>().operand.is<IntLiteral>());
}

TEST(Parsing, Nested_Unary_Operators_With_Parenthesis) {
    std::vector<Token> tokens = {
        { "-", "test.basalt", 1, 1, 1, Token::Type::symbol },
        { "-", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.get<UnaryOperator>().operand.is<IntLiteral>());
}