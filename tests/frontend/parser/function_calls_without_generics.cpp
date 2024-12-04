
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Frontend, Parse_Function_Call_WithOut_generics_And_Only_One_Argument_As_Statement) {
    std::vector<Token> tokens = {
        { "f", "test.basalt", 1, 1, 1, Token::Type::text   },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "x", "test.basalt", 1, 3, 3, Token::Type::text   },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { ";", "test.basalt", 1, 5, 5, Token::Type::symbol }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement proc = parser.parse_statement();
    ASSERT_TRUE(proc.is<FunctionCall>());
    EXPECT_EQ(proc.get<FunctionCall>().function_name, "f");
    ASSERT_EQ(proc.get<FunctionCall>().arguments.size(), 1);
    ASSERT_TRUE(proc.get<FunctionCall>().arguments.back().is<Identifier>());
    EXPECT_EQ(proc.get<FunctionCall>().arguments.back().get<Identifier>().name, "x");
}

TEST(Frontend, Parse_Function_Call_WithOut_generics_And_Two_Arguments_As_Statement) {
    std::vector<Token> tokens = {
        { "f", "test.basalt", 1, 1, 1, Token::Type::text   },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "x", "test.basalt", 1, 3, 3, Token::Type::text   },
        { ",", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "y", "test.basalt", 1, 5, 5, Token::Type::text   },
        { ")", "test.basalt", 1, 6, 6, Token::Type::symbol },
        { ";", "test.basalt", 1, 7, 7, Token::Type::symbol }
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Statement proc = parser.parse_statement();
    ASSERT_TRUE(proc.is<FunctionCall>());
    EXPECT_EQ(proc.get<FunctionCall>().function_name, "f");
    ASSERT_EQ(proc.get<FunctionCall>().arguments.size(), 2);
    ASSERT_TRUE(proc.get<FunctionCall>().arguments[0].is<Identifier>());
    EXPECT_EQ(proc.get<FunctionCall>().arguments[0].get<Identifier>().name, "x");
    ASSERT_TRUE(proc.get<FunctionCall>().arguments[1].is<Identifier>());
    EXPECT_EQ(proc.get<FunctionCall>().arguments[1].get<Identifier>().name, "y");
}

TEST(Frontend, Parse_Function_Call_WithOut_generics_And_Only_One_Argument_As_Expression) {
    std::vector<Token> tokens = {
        { "f", "test.basalt", 1, 1, 1, Token::Type::text   },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "x", "test.basalt", 1, 3, 3, Token::Type::text   },
        { ")", "test.basalt", 1, 4, 4, Token::Type::symbol },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<FunctionCall>());
    EXPECT_EQ(expr.get<FunctionCall>().function_name, "f");
    ASSERT_EQ(expr.get<FunctionCall>().arguments.size(), 1);
    ASSERT_TRUE(expr.get<FunctionCall>().arguments.back().is<Identifier>());
    EXPECT_EQ(expr.get<FunctionCall>().arguments.back().get<Identifier>().name, "x");
}

TEST(Frontend, Parse_Function_Call_WithOut_generics_And_Two_Arguments_As_Expression) {
    std::vector<Token> tokens = {
        { "f", "test.basalt", 1, 1, 1, Token::Type::text   },
        { "(", "test.basalt", 1, 2, 2, Token::Type::symbol },
        { "x", "test.basalt", 1, 3, 3, Token::Type::text   },
        { ",", "test.basalt", 1, 4, 4, Token::Type::symbol },
        { "y", "test.basalt", 1, 5, 5, Token::Type::text   },
        { ")", "test.basalt", 1, 6, 6, Token::Type::symbol },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    Expression expr = parser.parse_expression();    
    ASSERT_TRUE(expr.is<FunctionCall>());
    EXPECT_EQ(expr.get<FunctionCall>().function_name, "f");
    ASSERT_EQ(expr.get<FunctionCall>().arguments.size(), 2);
    ASSERT_TRUE(expr.get<FunctionCall>().arguments[0].is<Identifier>());
    EXPECT_EQ(expr.get<FunctionCall>().arguments[0].get<Identifier>().name, "x");
    ASSERT_TRUE(expr.get<FunctionCall>().arguments[1].is<Identifier>());
    EXPECT_EQ(expr.get<FunctionCall>().arguments[1].get<Identifier>().name, "y");
}