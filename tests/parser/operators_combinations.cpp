
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Operators_Combinations_minus_and_power) {
    std::vector<Token> tokens = {
        { "-", "test.basalt", 1, 1, 1, Token::Type::symbol },
        { "6", "test.basalt", 1, 2, 2, Token::Type::integer_literal },
        { "^", "test.basalt", 1, 3, 3, Token::Type::symbol },
        { "5", "test.basalt", 1, 4, 4, Token::Type::integer_literal }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.is<BinaryOperator>());
    ASSERT_EQ(expr.get<UnaryOperator>().operator_text, "-");
    ASSERT_EQ(expr.get<UnaryOperator>().operand.get<BinaryOperator>().operator_text, "^");
    ASSERT_EQ(expr.get<UnaryOperator>().operand.get<BinaryOperator>().left_operand.get<IntLiteral>().value, 6);
    ASSERT_EQ(expr.get<UnaryOperator>().operand.get<BinaryOperator>().right_operand.get<IntLiteral>().value, 5);
}

TEST(Parsing, Operators_Combinations_double_minus_and_power) {
    std::vector<Token> tokens = {
        { "-", "test.basalt", 0, 1, 1, Token::Type::symbol },
        { "-", "test.basalt", 0, 2, 2, Token::Type::symbol },
        { "6", "test.basalt", 0, 3, 3, Token::Type::integer_literal },
        { "^", "test.basalt", 0, 4, 4, Token::Type::symbol },
        { "5", "test.basalt", 0, 5, 5, Token::Type::integer_literal }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.get<UnaryOperator>().operand.is<BinaryOperator>());
    EXPECT_EQ(expr.get<UnaryOperator>().operator_text, "-");
    EXPECT_EQ(expr.get<UnaryOperator>().operand.get<UnaryOperator>().operator_text, "-");
    EXPECT_EQ(expr.get<UnaryOperator>().operand.get<UnaryOperator>().operand.get<BinaryOperator>().operator_text, "^");
}

TEST(Parsing, Operators_Combinations_double_minus_and_power_with_parenthesis) {
    std::vector<Token> tokens = {
        { "-", "test.basalt", 0, 1, 1, Token::Type::symbol },
        { "(", "test.basalt", 0, 2, 2, Token::Type::symbol },
        { "-", "test.basalt", 0, 3, 3, Token::Type::symbol },
        { "6", "test.basalt", 0, 4, 4, Token::Type::integer_literal },
        { ")", "test.basalt", 0, 5, 5, Token::Type::symbol },
        { "^", "test.basalt", 0, 6, 6, Token::Type::symbol },
        { "5", "test.basalt", 0, 7, 7, Token::Type::integer_literal }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.get<BinaryOperator>().left_operand.is<UnaryOperator>());
}