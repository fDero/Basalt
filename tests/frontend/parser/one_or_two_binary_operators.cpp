
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Simple_Binary_Operator) {
    std::vector<Token> tokens = {
        { "5", "test.basalt", 1, 1, 1, Token::Type::integer_literal },
        { "*", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "9", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().left_operand.is<IntLiteral>());
    ASSERT_TRUE(expr.get<BinaryOperator>().right_operand.is<IntLiteral>());
    EXPECT_EQ(expr.get<BinaryOperator>().operator_text, "*");
    EXPECT_EQ(expr.get<BinaryOperator>().left_operand.get<IntLiteral>().value, 5);
    EXPECT_EQ(expr.get<BinaryOperator>().right_operand.get<IntLiteral>().value, 9);
}

TEST(Parsing, Two_Binary_Operators_same_operator) {
    std::vector<Token> tokens = {
        { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal },
        { "*", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
        { "*", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "3", "test.basalt", 1, 5, 5, Token::Type::integer_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().left_operand.is<BinaryOperator>());
    BinaryOperator leftop = expr.get<BinaryOperator>().left_operand.get<BinaryOperator>();
    BinaryOperator parentop = expr.get<BinaryOperator>();
    ASSERT_TRUE(leftop.left_operand.is<IntLiteral>());
    ASSERT_TRUE(leftop.right_operand.is<IntLiteral>());
    ASSERT_TRUE(parentop.right_operand.is<IntLiteral>());
    EXPECT_EQ(parentop.operator_text, "*");
    EXPECT_EQ(leftop.operator_text, "*");
    EXPECT_EQ(leftop.left_operand.get<IntLiteral>().value, 1);
    EXPECT_EQ(leftop.right_operand.get<IntLiteral>().value, 2);
    EXPECT_EQ(parentop.right_operand.get<IntLiteral>().value, 3);
}

TEST(Parsing, Two_Binary_Operators_rotation_not_needed) {
    std::vector<Token> tokens = {
        { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal },
        { "*", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
        { "+", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "3", "test.basalt", 1, 5, 5, Token::Type::integer_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().left_operand.is<BinaryOperator>());
    BinaryOperator leftop = expr.get<BinaryOperator>().left_operand.get<BinaryOperator>();
    BinaryOperator parentop = expr.get<BinaryOperator>();
    ASSERT_TRUE(leftop.left_operand.is<IntLiteral>());
    ASSERT_TRUE(leftop.right_operand.is<IntLiteral>());
    ASSERT_TRUE(parentop.right_operand.is<IntLiteral>());
    EXPECT_EQ(parentop.operator_text, "+");
    EXPECT_EQ(leftop.operator_text, "*");
    EXPECT_EQ(leftop.left_operand.get<IntLiteral>().value, 1);
    EXPECT_EQ(leftop.right_operand.get<IntLiteral>().value, 2);
    EXPECT_EQ(parentop.right_operand.get<IntLiteral>().value, 3);
}

TEST(Parsing, Two_Binary_Operators_rotation_is_needed) {
    std::vector<Token> tokens = {
        { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal },
        { "+", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
        { "*", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "3", "test.basalt", 1, 5, 5, Token::Type::integer_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().right_operand.is<BinaryOperator>());
    BinaryOperator rightop = expr.get<BinaryOperator>().right_operand.get<BinaryOperator>();
    BinaryOperator parentop = expr.get<BinaryOperator>();
    ASSERT_TRUE(parentop.left_operand.is<IntLiteral>());
    ASSERT_TRUE(rightop.left_operand.is<IntLiteral>());
    ASSERT_TRUE(rightop.right_operand.is<IntLiteral>());
    EXPECT_EQ(parentop.operator_text, "+");
    EXPECT_EQ(rightop.operator_text, "*");
    EXPECT_EQ(parentop.left_operand.get<IntLiteral>().value, 1);
    EXPECT_EQ(rightop.left_operand.get<IntLiteral>().value, 2);
    EXPECT_EQ(rightop.right_operand.get<IntLiteral>().value, 3);
}

TEST(Parsing, Two_Binary_Operators_rotation_blocked_with_parenthesis) {
    std::vector<Token> tokens = {
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal },
        { "+", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
        { ")", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "*", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "3", "test.basalt", 1, 5, 5, Token::Type::integer_literal },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().left_operand.is<BinaryOperator>());
    BinaryOperator leftop = expr.get<BinaryOperator>().left_operand.get<BinaryOperator>();
    BinaryOperator parentop = expr.get<BinaryOperator>();
    ASSERT_TRUE(leftop.left_operand.is<IntLiteral>());
    ASSERT_TRUE(leftop.right_operand.is<IntLiteral>());
    ASSERT_TRUE(parentop.right_operand.is<IntLiteral>());
    EXPECT_EQ(parentop.operator_text, "*");
    EXPECT_EQ(leftop.operator_text, "+");
    EXPECT_EQ(leftop.left_operand.get<IntLiteral>().value, 1);
    EXPECT_EQ(leftop.right_operand.get<IntLiteral>().value, 2);
    EXPECT_EQ(parentop.right_operand.get<IntLiteral>().value, 3);
}