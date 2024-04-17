
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Simple_Array) {
    std::vector<Token> arraytokens = {
        { "[",   "test.basalt",  1, 1, 1, Token::Type::type               },
        { "10",  "test.basalt",  1, 2, 2, Token::Type::integer_literal    },
        { "]",   "test.basalt",  1, 3, 4, Token::Type::symbol             },
        { "Int", "test.basalt",  1, 4, 5, Token::Type::type               },
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
        { "[",   "test.basalt",     1, 1, 1, Token::Type::type            },
        { "10",  "test.basalt",     1, 2, 2, Token::Type::integer_literal },
        { "]",   "test.basalt",     1, 3, 4, Token::Type::symbol          },
        { "[",   "test.basalt",     1, 4, 5, Token::Type::type            },
        { "10",  "test.basalt",     1, 5, 7, Token::Type::integer_literal },
        { "]",   "test.basalt",     1, 6, 8, Token::Type::symbol          },
        { "Int", "test.basalt",     1, 7, 9, Token::Type::type            },
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