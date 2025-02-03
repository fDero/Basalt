
#include <gtest/gtest.h>
#include "frontend/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "syntax/primitive_types.hpp"

TEST(Frontend, Parse_Simple_Slice) {
    std::vector<Token> tokens = {
        { "$",   "test.basalt",   1, 1, 1, Token::Type::type     },
        { "Ent", "test.basalt",   1, 2, 2, Token::Type::type     },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<SliceType>());
    SliceType inner_slice = type.get<SliceType>();
    ASSERT_TRUE(inner_slice.stored_type.is<CustomType>());
    EXPECT_EQ(type.get<SliceType>().stored_type.get<CustomType>().type_name, "Ent");
}

TEST(Frontend, Parse_Nested_Slice_Types) {
    std::vector<Token> tokens = {
        { "$",   "test.basalt",     1, 1, 1,  Token::Type::type      },
        { "$",   "test.basalt",     1, 2, 2,  Token::Type::symbol    },
        { int_type, "test.basalt",     1, 3, 3,  Token::Type::type      },
    };
    Parser parser = Parser({ "inline-tests.basalt", tokens });
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<SliceType>());
    ASSERT_TRUE(type.get<SliceType>().stored_type.is<SliceType>());
    SliceType inner_slice = type.get<SliceType>().stored_type.get<SliceType>();
    ASSERT_TRUE(inner_slice.stored_type.is<PrimitiveType>());
    EXPECT_EQ(inner_slice.stored_type.get<PrimitiveType>().type_name, int_type);
}