
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/primitive_types.hpp"
#include "syntax/keywords.hpp"

TEST(Frontend, Parse_Is_Operator_Expression_On_Variable) {
    std::vector<Token> tokens = {
        { "number",      "test.basalt", 1, 1, 1, Token::Type::text       },
        { is_keyword,    "test.basalt", 1, 2, 7, Token::Type::is_keyword },
        { int_type,      "test.basalt", 1, 3, 9, Token::Type::type       },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expression = parser.parse_expression();
    ASSERT_TRUE(expression.is<TypeOperator>());
}

TEST(Frontend, Parse_As_Cast_On_Complex_Expression_AST_Rotation_Needed) {
    std::vector<Token> tokens = {
        { "x",         "test.basalt", 1, 1, 1, Token::Type::text       },
        { "*",         "test.basalt", 1, 2, 2, Token::Type::symbol     },
        { "y",         "test.basalt", 1, 3, 3, Token::Type::text       },
        { "+",         "test.basalt", 1, 4, 4, Token::Type::symbol     },
        { "z",         "test.basalt", 1, 5, 5, Token::Type::text       },
        { as_keyword,  "test.basalt", 1, 6, 7, Token::Type::as_keyword },
        { int_type,    "test.basalt", 1, 7, 9, Token::Type::type       },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expression = parser.parse_expression();
    ASSERT_TRUE(expression.is<BinaryOperator>());
    BinaryOperator binary_operator = expression.get<BinaryOperator>();
    ASSERT_TRUE(binary_operator.right_operand.is<TypeOperator>());
    TypeOperator type_operator = binary_operator.right_operand.get<TypeOperator>();
    ASSERT_TRUE(type_operator.typesignature.is<PrimitiveType>());
    PrimitiveType primitive_type = type_operator.typesignature.get<PrimitiveType>();
    EXPECT_EQ(primitive_type.type_name, int_type);
}

TEST(Frontend, Parse_Is_Operator_Expression_Used_As_Operand) {
    std::vector<Token> tokens = {
        { "number",      "test.basalt", 1, 1, 1,  Token::Type::text       },
        { is_keyword,    "test.basalt", 1, 2, 7,  Token::Type::is_keyword },
        { int_type,      "test.basalt", 1, 3, 9,  Token::Type::type       },
        { "&&",          "test.basalt", 1, 4, 12, Token::Type::symbol     },
        { "flag",        "test.basalt", 1, 5, 14, Token::Type::text       },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expression = parser.parse_expression();
    ASSERT_TRUE(expression.is<BinaryOperator>());
    BinaryOperator binary_operator = expression.get<BinaryOperator>();
    ASSERT_TRUE(binary_operator.left_operand.is<TypeOperator>());
}