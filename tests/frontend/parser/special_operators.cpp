
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/primitive_types.hpp"

TEST(Frontend, Parse_Dot_Member_Access_nested_with_multiplication) {
    std::vector<Token> tokens = {
        { "a", "test.basalt", 1, 1, 1, Token::Type::text },
        { "*", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "b", "test.basalt", 1, 3, 3, Token::Type::text },
        { ".", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "c", "test.basalt", 1, 5, 5, Token::Type::text },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    const BinaryOperator& binary_operator = expr.get<BinaryOperator>();
    EXPECT_TRUE(binary_operator.left_operand.is<Identifier>());
    ASSERT_TRUE(binary_operator.right_operand.is<DotMemberAccess>());
    const DotMemberAccess& dot_member_access = binary_operator.right_operand.get<DotMemberAccess>();
    EXPECT_TRUE(dot_member_access.struct_value.is<Identifier>());
    EXPECT_EQ(dot_member_access.member_name, "c");
}

TEST(Frontend, Parse_Dot_Member_Access_Array_Square_Brackets_Access_And_Pointer_Dereference_non_trivial_composition) {
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
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    const BinaryOperator& binary_operator = expr.get<BinaryOperator>();
    ASSERT_TRUE(binary_operator.left_operand.is<UnaryOperator>());
    const UnaryOperator& unary_operator = binary_operator.left_operand.get<UnaryOperator>();
    EXPECT_TRUE(unary_operator.operand.is<Identifier>());
    EXPECT_EQ(unary_operator.operator_text, "#");
    ASSERT_TRUE(binary_operator.right_operand.is<SquareBracketsAccess>());
    const SquareBracketsAccess& square_brackets_access = binary_operator.right_operand.get<SquareBracketsAccess>();
    EXPECT_TRUE(square_brackets_access.storage.is<DotMemberAccess>());
    const DotMemberAccess& dot_member_access = square_brackets_access.storage.get<DotMemberAccess>();
    EXPECT_TRUE(dot_member_access.struct_value.is<Identifier>());
    EXPECT_EQ(dot_member_access.member_name, "c");
}

TEST(Frontend, Parse_Dot_Member_Access_Array_Square_Brackets_Access_And_Pointer_Dereference_trivial_composition) {
    std::vector<Token> tokens = {
        { "#", "test.basalt", 1, 1, 1, Token::Type::symbol },
        { "a", "test.basalt", 1, 2, 2, Token::Type::text },
        { ".", "test.basalt", 1, 3, 3, Token::Type::symbol },
        { "b", "test.basalt", 1, 4, 4, Token::Type::text },
        { "[", "test.basalt", 1, 7, 7, Token::Type::symbol },
        { "k", "test.basalt", 1, 8, 8, Token::Type::text },
        { "]", "test.basalt", 1, 9, 9, Token::Type::symbol },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<UnaryOperator>());
    const UnaryOperator& unary_operator = expr.get<UnaryOperator>();
    EXPECT_TRUE(unary_operator.operand.is<SquareBracketsAccess>());
    const SquareBracketsAccess& square_brackets_access = unary_operator.operand.get<SquareBracketsAccess>();
    EXPECT_TRUE(square_brackets_access.storage.is<DotMemberAccess>());
    const DotMemberAccess& dot_member_access = square_brackets_access.storage.get<DotMemberAccess>();
    EXPECT_TRUE(dot_member_access.struct_value.is<Identifier>());
    EXPECT_EQ(dot_member_access.member_name, "b");
}