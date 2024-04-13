
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Dot_Member_Access_nested_with_multiplication){
    std::vector<Token> tokens = {
        { "a", "test.basalt", 1, 1, 1, Token::Type::text },
        { "*", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "b", "test.basalt", 1, 3, 3, Token::Type::text },
        { ".", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "c", "test.basalt", 1, 5, 5, Token::Type::text },
    };
    Parser parser = Parser(tokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().right_operand.is<BinaryOperator>());
    BinaryOperator parent = expr.get<BinaryOperator>();
    BinaryOperator rxop = parent.right_operand.get<BinaryOperator>();
    ASSERT_TRUE(parent.left_operand.is<Identifier>());
    ASSERT_TRUE(rxop.left_operand.is<Identifier>());
    ASSERT_TRUE(rxop.right_operand.is<Identifier>());
    EXPECT_EQ(parent.operator_text, "*");
    EXPECT_EQ(rxop.operator_text, ".");
    EXPECT_EQ(parent.left_operand.get<Identifier>().name, "a");
    EXPECT_EQ(rxop.left_operand.get<Identifier>().name, "b");
    EXPECT_EQ(rxop.right_operand.get<Identifier>().name, "c");
}

TEST(Parsing, Dot_Member_Access_Array_Square_Brackets_Access_And_Pointer_Dereference_non_trivial_composition){
    std::vector<Token> tokens = {
        { "#", "test.basalt", 1, 1, 1, Token::Type::symbol },
        { "a", "test.basalt", 1, 2, 2, Token::Type::text },
        { "*", "test.basalt", 1, 3, 3, Token::Type::symbol },
        { "b", "test.basalt", 1, 4, 4, Token::Type::text },
        { ".", "test.basalt", 1, 5, 5, Token::Type::symbol },
        { "c", "test.basalt", 1, 6, 6, Token::Type::text },
        { "[", "test.basalt", 1, 7, 7, Token::Type::symbol },
        { "k", "test.basalt", 1, 8, 8, Token::Type::text },
        { "]", "test.basalt", 1, 9, 9, Token::Type::symbol },
    };
    Parser parser = Parser(tokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().right_operand.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().left_operand.is<UnaryOperator>());
    BinaryOperator parent = expr.get<BinaryOperator>();
    BinaryOperator rightop = parent.right_operand.get<BinaryOperator>();
    UnaryOperator leftop = parent.left_operand.get<UnaryOperator>();
    ASSERT_TRUE(rightop.left_operand.is<BinaryOperator>());
    BinaryOperator dotop = rightop.left_operand.get<BinaryOperator>();
    EXPECT_EQ(parent.operator_text, "*");
    EXPECT_EQ(leftop.operator_text, "#");
    EXPECT_EQ(rightop.operator_text, "[square-brackets-access]");
    EXPECT_EQ(dotop.operator_text, ".");
}

TEST(Parsing, Dot_Member_Access_Array_Square_Brackets_Access_And_Pointer_Dereference_trivial_composition){
    std::vector<Token> tokens = {
        { "#", "test.basalt", 1, 1, 1, Token::Type::symbol },
        { "a", "test.basalt", 1, 2, 2, Token::Type::text },
        { ".", "test.basalt", 1, 3, 3, Token::Type::symbol },
        { "b", "test.basalt", 1, 4, 4, Token::Type::text },
        { "[", "test.basalt", 1, 7, 7, Token::Type::symbol },
        { "k", "test.basalt", 1, 8, 8, Token::Type::text },
        { "]", "test.basalt", 1, 9, 9, Token::Type::symbol },
    };
    Parser parser = Parser(tokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<UnaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<UnaryOperator>().operand.get<BinaryOperator>().left_operand.is<BinaryOperator>());
    UnaryOperator parent = expr.get<UnaryOperator>();
    BinaryOperator sqop = parent.operand.get<BinaryOperator>();
    BinaryOperator dotop = sqop.left_operand.get<BinaryOperator>();
    EXPECT_EQ(parent.operator_text, "#");
    EXPECT_EQ(sqop.operator_text, "[square-brackets-access]");
    EXPECT_EQ(dotop.operator_text, ".");
}