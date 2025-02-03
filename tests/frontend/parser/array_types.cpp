
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/primitive_types.hpp"

TEST(Frontend, Parse_Simple_Array) {
    std::vector<Token> tokens = {
        { "[",   "test.basalt",  1, 1, 1, Token::Type::type               },
        { "10",  "test.basalt",  1, 2, 2, Token::Type::integer_literal    },
        { "]",   "test.basalt",  1, 3, 4, Token::Type::symbol             },
        { int_type, "test.basalt",  1, 4, 5, Token::Type::type               },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<ArrayType>());
    EXPECT_EQ(type.get<ArrayType>().array_length, 10);
    ASSERT_TRUE(type.get<ArrayType>().stored_type.is<PrimitiveType>());
    EXPECT_EQ(type.get<ArrayType>().stored_type.get<PrimitiveType>().type_name, int_type);
}

TEST(Frontend, Parse_Nested_Array_Types) {
    std::vector<Token> tokens = {
        { "[",   "test.basalt",     1, 1, 1, Token::Type::type            },
        { "10",  "test.basalt",     1, 2, 2, Token::Type::integer_literal },
        { "]",   "test.basalt",     1, 3, 4, Token::Type::symbol          },
        { "[",   "test.basalt",     1, 4, 5, Token::Type::type            },
        { "10",  "test.basalt",     1, 5, 7, Token::Type::integer_literal },
        { "]",   "test.basalt",     1, 6, 8, Token::Type::symbol          },
        { "Ent", "test.basalt",     1, 7, 9, Token::Type::type            },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<ArrayType>());
    ASSERT_TRUE(type.get<ArrayType>().stored_type.is<ArrayType>());
    ArrayType outer_array = type.get<ArrayType>();
    ArrayType inner_array = type.get<ArrayType>().stored_type.get<ArrayType>();
    EXPECT_EQ(outer_array.array_length, 10);
    EXPECT_EQ(inner_array.array_length, 10);
    ASSERT_TRUE(inner_array.stored_type.is<CustomType>());
    EXPECT_EQ(inner_array.stored_type.get<CustomType>().type_name, "Ent");
}