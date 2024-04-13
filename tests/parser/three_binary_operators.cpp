
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Three_Binary_Operators_two_rotations_needed){
    std::vector<Token> tokens = {
        { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal },
        { "+", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal },
        { "*", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "3", "test.basalt", 1, 5, 5, Token::Type::integer_literal },
        { "^", "test.basalt", 1, 6, 6, Token::Type::symbol },
        { "4", "test.basalt", 1, 7, 7, Token::Type::integer_literal },
    };
    Parser parser = Parser(tokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().right_operand.is<BinaryOperator>());
    ASSERT_TRUE(expr.get<BinaryOperator>().right_operand.get<BinaryOperator>().right_operand.is<BinaryOperator>());
    BinaryOperator parent = expr.get<BinaryOperator>();
    BinaryOperator rxop = expr.get<BinaryOperator>().right_operand.get<BinaryOperator>();
    BinaryOperator rxrxop = expr.get<BinaryOperator>().right_operand.get<BinaryOperator>().right_operand.get<BinaryOperator>();
    EXPECT_EQ(parent.operator_text, "+");
    EXPECT_EQ(rxop.operator_text, "*");
    EXPECT_EQ(rxrxop.operator_text, "^");
    EXPECT_EQ(parent.left_operand.get<IntLiteral>().value, 1);
    EXPECT_EQ(rxop.left_operand.get<IntLiteral>().value, 2);
    EXPECT_EQ(rxrxop.left_operand.get<IntLiteral>().value, 3);
    EXPECT_EQ(rxrxop.right_operand.get<IntLiteral>().value, 4);
}