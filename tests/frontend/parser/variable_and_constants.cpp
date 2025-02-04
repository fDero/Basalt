
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/primitive_types.hpp"
#include "syntax/keywords.hpp"

TEST(Frontend, Parse_Variable_With_Type_And_Value) {
    std::vector<Token> tokens = {
        { var_keyword, "test.basalt", 1, 1, 1,  Token::Type::var_keyword },
        { "x",   "test.basalt", 1, 2, 4,  Token::Type::text              },
        { ":",   "test.basalt", 1, 3, 5,  Token::Type::symbol            },
        { int_type, "test.basalt", 1, 4, 8,  Token::Type::type           },
        { "=",   "test.basalt", 1, 5, 9,  Token::Type::symbol            },
        { "6",   "test.basalt", 1, 6, 10, Token::Type::integer_literal   },
        { ";",   "test.basalt", 1, 7, 11, Token::Type::symbol            },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_variable_definition();
    ASSERT_TRUE(statement.is<VariableDeclaration>());
    VariableDeclaration var = statement.get<VariableDeclaration>();
    EXPECT_EQ(var.identifier_name, "x");
    ASSERT_TRUE(var.typesignature.is<PrimitiveType>());
    ASSERT_TRUE(var.initial_value.has_value());
}

TEST(Frontend, Parse_Variable_With_No_Value) {
    std::vector<Token> tokens = {
        { var_keyword, "test.basalt", 1, 1, 1,  Token::Type::var_keyword },
        { "x",         "test.basalt", 1, 2, 4,  Token::Type::text        },
        { ":",         "test.basalt", 1, 3, 5,  Token::Type::symbol      },
        { int_type,    "test.basalt", 1, 4, 8,  Token::Type::type        },
        { ";",         "test.basalt", 1, 7, 11, Token::Type::symbol      },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_variable_definition();
    ASSERT_TRUE(statement.is<VariableDeclaration>());
    VariableDeclaration var = statement.get<VariableDeclaration>();
    EXPECT_EQ(var.identifier_name, "x");
    ASSERT_TRUE(var.typesignature.is<PrimitiveType>());
    ASSERT_FALSE(var.initial_value.has_value());
}

TEST(Frontend, Parse_Constant_With_Type_And_Value) {
    std::vector<Token> tokens = {
        { const_keyword, "test.basalt", 1, 1, 1,  Token::Type::const_keyword },
        { "x",   "test.basalt",   1, 2, 6,  Token::Type::text                },
        { ":",   "test.basalt",   1, 3, 8,  Token::Type::symbol              },
        { int_type, "test.basalt",   1, 4, 10, Token::Type::type             },
        { "=",   "test.basalt",   1, 5, 11, Token::Type::symbol              },
        { "6",   "test.basalt",   1, 6, 12, Token::Type::integer_literal     },
        { ";",   "test.basalt",   1, 7, 13, Token::Type::symbol              },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement statement = parser.parse_constant_definition();
    ASSERT_TRUE(statement.is<ConstDeclaration>());
    ConstDeclaration con = statement.get<ConstDeclaration>();
    EXPECT_EQ(con.identifier_name, "x");
    ASSERT_TRUE(con.typesignature.is<PrimitiveType>());
}
