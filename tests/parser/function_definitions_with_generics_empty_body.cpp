
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Function_Definition_With_generics_And_No_Arguments_And_Empty_Body) {
    std::vector<Token> function_def_tokens = {
        { "func","test.basalt", 1, 1, 1, Token::Type::func_keyword },
        { "f",   "test.basalt", 1, 2, 2, Token::Type::text   },
        { "<",   "test.basalt", 1, 3, 3, Token::Type::symbol },
        { "T",   "test.basalt", 1, 4, 4, Token::Type::type },
        { ">",   "test.basalt", 1, 5, 5, Token::Type::symbol },
        { "(",   "test.basalt", 1, 6, 6, Token::Type::symbol },
        { ")",   "test.basalt", 1, 7, 7, Token::Type::symbol },
        { "{",   "test.basalt", 1, 8, 8, Token::Type::symbol },
        { "}",   "test.basalt", 1, 9, 9, Token::Type::symbol }
    };
    Parser parser = Parser(function_def_tokens);
    FunctionDefinition funcdef = parser.parse_function_definition();
    EXPECT_EQ(funcdef.function_name, "f");    
    EXPECT_TRUE(funcdef.arguments.empty());
    EXPECT_TRUE(funcdef.code.empty());
    EXPECT_EQ(funcdef.filename, "test.basalt");
    ASSERT_EQ(funcdef.template_generics_names.size(), 1);
    EXPECT_EQ(funcdef.template_generics_names.back(), "T");
}

TEST(Parsing, Function_Definition_With_generics_And_Only_One_Argument_And_Empty_Body) {
    std::vector<Token> function_def_tokens = {
        { "func","test.basalt", 1, 1, 1,  Token::Type::func_keyword },
        { "f",   "test.basalt", 1, 2, 5,  Token::Type::text   },
        { "<",   "test.basalt", 1, 3, 3,  Token::Type::symbol },
        { "T",   "test.basalt", 1, 4, 4,  Token::Type::type },
        { ",",   "test.basalt", 1, 3, 3,  Token::Type::symbol },
        { "U",   "test.basalt", 1, 4, 4,  Token::Type::type },
        { ">",   "test.basalt", 1, 5, 5,  Token::Type::symbol },
        { "(",   "test.basalt", 1, 3, 6,  Token::Type::symbol },
        { "x",   "test.basalt", 1, 4, 7,  Token::Type::text   },
        { ":",   "test.basalt", 1, 5, 8,  Token::Type::symbol },
        { "Int", "test.basalt", 1, 6, 9,  Token::Type::type   },
        { ")",   "test.basalt", 1, 7, 13, Token::Type::symbol },
        { "{",   "test.basalt", 1, 8, 14, Token::Type::symbol },
        { "}",   "test.basalt", 1, 9, 15, Token::Type::symbol }
    };
    Parser parser = Parser(function_def_tokens);
    FunctionDefinition funcdef = parser.parse_function_definition();
    EXPECT_EQ(funcdef.function_name, "f");
    EXPECT_TRUE(funcdef.code.empty());
    EXPECT_EQ(funcdef.filename, "test.basalt");
    ASSERT_EQ(funcdef.arguments.size(), 1);    
    EXPECT_EQ(funcdef.arguments[0].arg_name, "x");
    ASSERT_TRUE(funcdef.arguments[0].arg_type.is<BaseType>());
    EXPECT_EQ(funcdef.arguments[0].arg_type.get<BaseType>().type_name, "Int");
    ASSERT_EQ(funcdef.template_generics_names.size(), 2);
    EXPECT_EQ(funcdef.template_generics_names.back(), "U");
    EXPECT_EQ(funcdef.template_generics_names.front(), "T");
}