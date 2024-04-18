
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Simple_Slice) {
    std::vector<Token> slicetokens = {
        { "$", "test.basalt",     1, 1, 1,  Token::Type::type     },
        { "Int", "test.basalt",   1, 2, 2, Token::Type::type     },
    };
    Parser parser = Parser(slicetokens);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<SliceType>());
    SliceType inner_slice = type.get<SliceType>();
    ASSERT_TRUE(inner_slice.stored_type.is<PrimitiveType>());
    EXPECT_EQ(type.get<SliceType>().stored_type.get<PrimitiveType>().type_name, "Int");
}

TEST(Parsing, Nested_Slice_Types) {
    std::vector<Token> slicetokens = {
        { "$",   "test.basalt",     1, 1, 1,  Token::Type::type      },
        { "$",   "test.basalt",     1, 2, 2,  Token::Type::symbol    },
        { "Int", "test.basalt",     1, 3, 3,  Token::Type::type      },
    };
    Parser parser = Parser(slicetokens);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<SliceType>());
    ASSERT_TRUE(type.get<SliceType>().stored_type.is<SliceType>());
    SliceType inner_slice = type.get<SliceType>().stored_type.get<SliceType>();
    ASSERT_TRUE(inner_slice.stored_type.is<PrimitiveType>());
    EXPECT_EQ(inner_slice.stored_type.get<PrimitiveType>().type_name, "Int");
}