
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Array_Literal_Without_Explicit_Size) {
    std::vector<Token> arraytokens = {
        { "[",   "test.basalt",  1, 1, 1, Token::Type::symbol  },
        { "]",   "test.basalt",  1, 2, 2, Token::Type::symbol  },
        { "Ent", "test.basalt",  1, 3, 3, Token::Type::type    },
        { "{",   "test.basalt",  1, 4, 5, Token::Type::symbol  },
        { "}",   "test.basalt",  1, 5, 6, Token::Type::symbol  },
    };
    Parser parser = Parser(arraytokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<ArrayLiteral>());
    EXPECT_EQ(expr.get<ArrayLiteral>().array_length, -1);
    EXPECT_TRUE(expr.get<ArrayLiteral>().elements.empty());
    ASSERT_TRUE(expr.get<ArrayLiteral>().stored_type.is<BaseType>());
    EXPECT_EQ(expr.get<ArrayLiteral>().stored_type.get<BaseType>().type_name, "Ent");
    EXPECT_TRUE(expr.get<ArrayLiteral>().stored_type.get<BaseType>().instantiationd_generics.empty());
}

TEST(Parsing, Array_Literal_Of_Generic_StoredType_Without_Explicit_Size) {
    std::vector<Token> arraytokens = {
        { "[",    "test.basalt",  1, 1, 1, Token::Type::symbol  },
        { "]",    "test.basalt",  1, 2, 2, Token::Type::symbol  },
        { "Pair", "test.basalt",  1, 3, 3, Token::Type::type    },
        { "<",    "test.basalt",  1, 4, 5, Token::Type::symbol  },
        { "Int",  "test.basalt",  1, 3, 3, Token::Type::type    },
        { ",",    "test.basalt",  1, 4, 5, Token::Type::symbol  },
        { "Int",  "test.basalt",  1, 3, 3, Token::Type::type    },
        { ">",    "test.basalt",  1, 5, 6, Token::Type::symbol  },
        { "{",    "test.basalt",  1, 4, 5, Token::Type::symbol  },
        { "}",    "test.basalt",  1, 5, 6, Token::Type::symbol  },
    };
    Parser parser = Parser(arraytokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<ArrayLiteral>());
    EXPECT_EQ(expr.get<ArrayLiteral>().array_length, -1);
    EXPECT_TRUE(expr.get<ArrayLiteral>().elements.empty());
    ASSERT_TRUE(expr.get<ArrayLiteral>().stored_type.is<BaseType>());
    EXPECT_EQ(expr.get<ArrayLiteral>().stored_type.get<BaseType>().type_name, "Pair");
    EXPECT_EQ(expr.get<ArrayLiteral>().stored_type.get<BaseType>().instantiationd_generics.size(), 2);
}

TEST(Parsing, Nested_Array_Literals_Without_Explicit_Size_And_Incorrect_Type) {
    std::vector<Token> arraytokens = {
        { "[",   "test.basalt",  1,  1,  1, Token::Type::symbol  },
        { "]",   "test.basalt",  1,  2,  2, Token::Type::symbol  },
        { "Int", "test.basalt",  1,  3,  3, Token::Type::type    },
        { "{",   "test.basalt",  1,  4,  5, Token::Type::symbol  },

        { "[",   "test.basalt",  1,  5,  6, Token::Type::symbol  },
        { "]",   "test.basalt",  1,  6,  7, Token::Type::symbol  },
        { "Int", "test.basalt",  1,  7,  8, Token::Type::type    },
        { "{",   "test.basalt",  1,  8, 11, Token::Type::symbol  },
        { "}",   "test.basalt",  1,  8, 11, Token::Type::symbol  },
        
        { ",",   "test.basalt",  1,  9, 12, Token::Type::symbol  },
        { "[",   "test.basalt",  1, 10, 13, Token::Type::symbol  },
        { "]",   "test.basalt",  1, 11, 14, Token::Type::symbol  },
        { "Int", "test.basalt",  1, 12, 17, Token::Type::type    },
        { "{",   "test.basalt",  1, 13, 18, Token::Type::symbol  },
        { "}",   "test.basalt",  1, 14, 19, Token::Type::symbol  },
        
        { "}",   "test.basalt",  1, 15, 20, Token::Type::symbol  },
    };
    Parser parser = Parser(arraytokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<ArrayLiteral>());
    EXPECT_EQ(expr.get<ArrayLiteral>().array_length, -1);
    EXPECT_EQ(expr.get<ArrayLiteral>().elements.size(), 2);
    ASSERT_TRUE(expr.get<ArrayLiteral>().stored_type.is<PrimitiveType>());
    EXPECT_EQ(expr.get<ArrayLiteral>().stored_type.get<PrimitiveType>().type_name, "Int");

    for (Expression inner_array : expr.get<ArrayLiteral>().elements){
        ASSERT_TRUE(inner_array.is<ArrayLiteral>());
        EXPECT_EQ(inner_array.get<ArrayLiteral>().array_length, -1);
        EXPECT_TRUE(inner_array.get<ArrayLiteral>().elements.empty());
        ASSERT_TRUE(inner_array.get<ArrayLiteral>().stored_type.is<PrimitiveType>());
        EXPECT_EQ(inner_array.get<ArrayLiteral>().stored_type.get<PrimitiveType>().type_name, "Int");
    }
}

TEST(Parsing, Nested_Array_Literals_With_Explicit_Size_And_Correct_Type) {
    std::vector<Token> arraytokens = {
        { "[",   "test.basalt",  1,  1,  1, Token::Type::symbol  },
        { "2",   "test.basalt",  1,  1,  1, Token::Type::integer_literal  },
        { "]",   "test.basalt",  1,  2,  2, Token::Type::symbol  },
        { "[",   "test.basalt",  1,  1,  1, Token::Type::symbol  },
        { "0",   "test.basalt",  1,  1,  1, Token::Type::integer_literal  },
        { "]",   "test.basalt",  1,  2,  2, Token::Type::symbol  },
        { "Int", "test.basalt",  1,  3,  3, Token::Type::type    },
        { "{",   "test.basalt",  1,  4,  5, Token::Type::symbol  },
        { "[",   "test.basalt",  1,  5,  6, Token::Type::symbol  },
        { "]",   "test.basalt",  1,  6,  7, Token::Type::symbol  },
        { "Int", "test.basalt",  1,  7,  8, Token::Type::type    },
        { "{",   "test.basalt",  1,  8, 11, Token::Type::symbol  },
        { "}",   "test.basalt",  1,  8, 11, Token::Type::symbol  },
        { ",",   "test.basalt",  1,  9, 12, Token::Type::symbol  },
        { "[",   "test.basalt",  1, 10, 13, Token::Type::symbol  },
        { "]",   "test.basalt",  1, 11, 14, Token::Type::symbol  },
        { "Int", "test.basalt",  1, 12, 17, Token::Type::type    },
        { "{",   "test.basalt",  1, 13, 18, Token::Type::symbol  },
        { "}",   "test.basalt",  1, 14, 19, Token::Type::symbol  },
        { "}",   "test.basalt",  1, 15, 20, Token::Type::symbol  },
    };
    Parser parser = Parser(arraytokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<ArrayLiteral>());
    EXPECT_EQ(expr.get<ArrayLiteral>().array_length, 2);
    EXPECT_EQ(expr.get<ArrayLiteral>().elements.size(), 2);
    ASSERT_TRUE(expr.get<ArrayLiteral>().stored_type.is<ArrayType>());

    for (Expression inner_array : expr.get<ArrayLiteral>().elements){
        ASSERT_TRUE(inner_array.is<ArrayLiteral>());
        EXPECT_EQ(inner_array.get<ArrayLiteral>().array_length, -1);
        EXPECT_TRUE(inner_array.get<ArrayLiteral>().elements.empty());
        ASSERT_TRUE(inner_array.get<ArrayLiteral>().stored_type.is<PrimitiveType>());
        EXPECT_EQ(inner_array.get<ArrayLiteral>().stored_type.get<PrimitiveType>().type_name, "Int");
    }
}

TEST(Parsing, Square_Bracket_Access_On_Array_Literal) {
    std::vector<Token> arraytokens = {
        { "[",   "test.basalt",  1,  1,  1, Token::Type::symbol  },
        { "]",   "test.basalt",  1,  2,  2, Token::Type::symbol  },
        { "Int", "test.basalt",  1,  3,  3, Token::Type::type    },
        { "{",   "test.basalt",  1,  4,  6, Token::Type::symbol  },
        { "}",   "test.basalt",  1,  5,  7, Token::Type::symbol  },
        { "[",   "test.basalt",  1,  6,  8, Token::Type::symbol  },
        { "2",   "test.basalt",  1,  6,  9, Token::Type::integer_literal  },
        { "]",   "test.basalt",  1,  7, 10, Token::Type::symbol  },
    };
    Parser parser = Parser(arraytokens);
    Expression expr = parser.parse_expression();
    ASSERT_TRUE(expr.is<BinaryOperator>());
    ASSERT_EQ(expr.get<BinaryOperator>().operator_text, "[square-brackets-access]");
    Expression array_literal = expr.get<BinaryOperator>().left_operand;
    Expression index = expr.get<BinaryOperator>().right_operand;

    ASSERT_TRUE(array_literal.is<ArrayLiteral>());
    EXPECT_EQ(array_literal.get<ArrayLiteral>().array_length, -1);
    EXPECT_TRUE(array_literal.get<ArrayLiteral>().elements.empty());
    ASSERT_TRUE(array_literal.get<ArrayLiteral>().stored_type.is<PrimitiveType>());
    EXPECT_EQ(array_literal.get<ArrayLiteral>().stored_type.get<PrimitiveType>().type_name, "Int");

    ASSERT_TRUE(index.is<IntLiteral>());
    EXPECT_EQ(index.get<IntLiteral>().value, 2);
}