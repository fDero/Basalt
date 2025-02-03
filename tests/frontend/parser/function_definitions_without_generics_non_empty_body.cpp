
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/primitive_types.hpp"
#include "syntax/keywords.hpp"

TEST(Frontend, Parse_Function_Definition_WithOut_generics_And_No_Arguments_And_NON_Empty_Body) {
    std::vector<Token> tokens = {
        { func_keyword, "test.basalt", 1,  1,  1,   Token::Type::func_keyword },
        { "f",          "test.basalt", 1,  2,  5,   Token::Type::text         },
        { "(",          "test.basalt", 1,  3,  6,   Token::Type::symbol       },
        { ")",          "test.basalt", 1,  4,  7,   Token::Type::symbol       },
        { "{",          "test.basalt", 1,  5,  8,   Token::Type::symbol       },
        { var_keyword,  "test.basalt", 1,  8,  14,  Token::Type::var_keyword  },
        { "x",          "test.basalt", 1,  8,  14,  Token::Type::text         },
        { ":",          "test.basalt", 1,  8,  14,  Token::Type::symbol       },
        { int_type,     "test.basalt", 1,  8,  14,  Token::Type::type         },
        { ";",          "test.basalt", 1,  8,  14,  Token::Type::symbol       },
        { "}",          "test.basalt", 1,  6,  9,   Token::Type::symbol       }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    FunctionDefinition funcdef = parser.parse_function_definition();
    EXPECT_EQ(funcdef.function_name, "f");    
    EXPECT_TRUE(funcdef.arguments.empty());
    EXPECT_EQ(funcdef.filename, "test.basalt");
    ASSERT_EQ(funcdef.code.size(), 1);
    ASSERT_TRUE(funcdef.code.back().is<VariableDeclaration>());
}

TEST(Frontend, Parse_Function_Definition_WithOut_generics_And_Only_One_Argument_And_NON_Empty_Body) {
    std::vector<Token> tokens = {
        { func_keyword,    "test.basalt", 1,  1,  1,   Token::Type::func_keyword          },
        { "f",             "test.basalt", 1,  2,  5,   Token::Type::text                  },
        { "(",             "test.basalt", 1,  3,  6,   Token::Type::symbol                },
        { "x",             "test.basalt", 1,  4,  7,   Token::Type::text                  },
        { ":",             "test.basalt", 1,  5,  8,   Token::Type::symbol                },
        { int_type,        "test.basalt", 1,  6,  9,   Token::Type::type                  },
        { ")",             "test.basalt", 1,  7,  13,  Token::Type::symbol                },
        { "{",             "test.basalt", 1,  8,  14,  Token::Type::symbol                },
        { "x",             "test.basalt", 1,  8,  14,  Token::Type::text                  },
        { "=",             "test.basalt", 1,  8,  14,  Token::Type::symbol                },
        { "6",             "test.basalt", 1,  8,  14,  Token::Type::integer_literal       },
        { ";",             "test.basalt", 1,  8,  14,  Token::Type::symbol                },
        { return_keyword,        "test.basalt", 1, 13,  18,  Token::Type::return_keyword  },
        { "x",             "test.basalt", 1, 13,  18,  Token::Type::text                  },
        { ";",             "test.basalt", 1, 14,  24,  Token::Type::symbol                },
        { "}",             "test.basalt", 1,  9,  15,  Token::Type::symbol                }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    FunctionDefinition funcdef = parser.parse_function_definition();
    EXPECT_EQ(funcdef.function_name, "f");
    EXPECT_EQ(funcdef.filename, "test.basalt");
    ASSERT_EQ(funcdef.arguments.size(), 1);    
    EXPECT_EQ(funcdef.arguments[0].arg_name, "x");
    ASSERT_TRUE(funcdef.arguments[0].arg_type.is<PrimitiveType>());
    EXPECT_EQ(funcdef.arguments[0].arg_type.get<PrimitiveType>().type_name, int_type);
    ASSERT_EQ(funcdef.code.size(), 2);
    ASSERT_TRUE(funcdef.code.back().is<Return>());
    EXPECT_TRUE(funcdef.code.back().get<Return>().return_value.has_value());
    ASSERT_TRUE(funcdef.code.front().is<Assignment>());
}

TEST(Frontend, Parse_Function_Definition_WithOut_generics_And_Two_Arguments_And_NON_Empty_Body) {
    std::vector<Token> tokens = {
        { func_keyword,    "test.basalt", 1, 1,  1,   Token::Type::func_keyword         },
        { "f",             "test.basalt", 1, 2,  5,   Token::Type::text                 },
        { "(",             "test.basalt", 1, 3,  6,   Token::Type::symbol               },
        { "x",             "test.basalt", 1, 4,  7,   Token::Type::text                 },
        { ":",             "test.basalt", 1, 5,  8,   Token::Type::symbol               },
        { int_type,        "test.basalt", 1, 6,  9,   Token::Type::type                 },
        { ",",             "test.basalt", 1, 7,  10,  Token::Type::type                 },
        { "y",             "test.basalt", 1, 8,  11,  Token::Type::text                 },
        { ":",             "test.basalt", 1, 9,  12,  Token::Type::symbol               },
        { int_type,        "test.basalt", 1, 10, 13,  Token::Type::type                 },
        { ")",             "test.basalt", 1, 11, 16,  Token::Type::symbol               },
        { "{",             "test.basalt", 1, 12, 17,  Token::Type::symbol               },
        { return_keyword,  "test.basalt", 1, 13, 18,  Token::Type::return_keyword },
        { ";",             "test.basalt", 1, 14, 24,  Token::Type::symbol               },
        { "}",             "test.basalt", 1, 15, 25,  Token::Type::symbol               }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    FunctionDefinition funcdef = parser.parse_function_definition();
    EXPECT_EQ(funcdef.function_name, "f");    
    EXPECT_EQ(funcdef.filename, "test.basalt");
    ASSERT_EQ(funcdef.arguments.size(), 2);    
    EXPECT_EQ(funcdef.arguments[0].arg_name, "x");
    ASSERT_TRUE(funcdef.arguments[0].arg_type.is<PrimitiveType>());
    EXPECT_EQ(funcdef.arguments[0].arg_type.get<PrimitiveType>().type_name, int_type);
    EXPECT_EQ(funcdef.arguments[1].arg_name, "y");
    ASSERT_TRUE(funcdef.arguments[1].arg_type.is<PrimitiveType>());
    EXPECT_EQ(funcdef.arguments[1].arg_type.get<PrimitiveType>().type_name, int_type);
    ASSERT_EQ(funcdef.code.size(), 1);
    EXPECT_TRUE(funcdef.code.back().is<Return>());
}
