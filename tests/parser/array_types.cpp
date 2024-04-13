
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Simple_Array) {
    std::vector<Token> arraytokens = {
        { "Array", "test.basalt", 1, 1, 1,  Token::Type::type     },
        { "<", "test.basalt",     1, 2, 6,  Token::Type::symbol   },
        { "10", "test.basalt",    1, 4, 11, Token::Type::integer_literal },
        { ",", "test.basalt",     1, 4, 11, Token::Type::symbol   },
        { "Int", "test.basalt",   1, 3, 10, Token::Type::type     },
        { ">", "test.basalt",     1, 6, 14, Token::Type::symbol   },
    };
    Parser parser = Parser(arraytokens);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<ArrayType>());
    EXPECT_EQ(type.get<ArrayType>().array_length, 10);
    ASSERT_TRUE(type.get<ArrayType>().stored_type.is<BaseType>());
    EXPECT_EQ(type.get<ArrayType>().stored_type.get<BaseType>().type_name, "Int");
}

TEST(Parsing, Nested_Array_Types) {
    std::vector<Token> arraytokens = {
        { "Array", "test.basalt", 1,  1,  1, Token::Type::type     },
        { "<", "test.basalt",     1,  2,  6, Token::Type::symbol   },
        { "10", "test.basalt",    1,  3,  8, Token::Type::integer_literal },
        { ",", "test.basalt",     1,  4,  9, Token::Type::symbol   },
        { "Array", "test.basalt", 1,  5, 15, Token::Type::type     },
        { "<", "test.basalt",     1,  6, 16, Token::Type::symbol   },
        { "10", "test.basalt",    1,  7, 18, Token::Type::integer_literal },
        { ",", "test.basalt",     1,  8, 19, Token::Type::symbol   },
        { "Int", "test.basalt",   1,  9, 22, Token::Type::type     },
        { ">", "test.basalt",     1, 10, 23, Token::Type::symbol   },
        { ">", "test.basalt",     1, 11, 24, Token::Type::symbol   },
    };
    Parser parser = Parser(arraytokens);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<ArrayType>());
    ASSERT_TRUE(type.get<ArrayType>().stored_type.is<ArrayType>());
    ArrayType outer_array = type.get<ArrayType>();
    ArrayType inner_array = type.get<ArrayType>().stored_type.get<ArrayType>();
    EXPECT_EQ(outer_array.array_length, 10);
    EXPECT_EQ(inner_array.array_length, 10);
    ASSERT_TRUE(inner_array.stored_type.is<BaseType>());
    EXPECT_EQ(inner_array.stored_type.get<BaseType>().type_name, "Int");
    EXPECT_TRUE(inner_array.stored_type.get<BaseType>().instanciated_generics.empty());
}