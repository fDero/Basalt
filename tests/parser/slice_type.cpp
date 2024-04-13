
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Simple_Slice) {
    std::vector<Token> slicetokens = {
        { "Slice", "test.basalt", 1, 1, 1,  Token::Type::type     },
        { "<", "test.basalt",     1, 2, 6,  Token::Type::symbol   },
        { "Int", "test.basalt",   1, 3, 10, Token::Type::type     },
        { ">", "test.basalt",     1, 6, 11, Token::Type::symbol   },
    };
    Parser parser = Parser(slicetokens);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<SliceType>());
    EXPECT_EQ(type.get<SliceType>().stored_type.get<BaseType>().type_name, "Int");
}

TEST(Parsing, Nested_Slice_Types) {
    std::vector<Token> slicetokens = {
        { "Slice", "test.basalt", 1, 1, 1,  Token::Type::type     },
        { "<", "test.basalt",     1, 2, 6,  Token::Type::symbol   },
        { "Slice", "test.basalt", 1, 1, 7,  Token::Type::type     },
        { "<", "test.basalt",     1, 2, 8,  Token::Type::symbol   },
        { "Int", "test.basalt",   1, 3, 9, Token::Type::type     },
        { ">", "test.basalt",     1, 4, 13, Token::Type::symbol   },
        { ">", "test.basalt",     1, 5, 14, Token::Type::symbol   },
    };
    Parser parser = Parser(slicetokens);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<SliceType>());
    ASSERT_TRUE(type.get<SliceType>().stored_type.is<SliceType>());
    SliceType inner_slice = type.get<SliceType>().stored_type.get<SliceType>();
    ASSERT_TRUE(inner_slice.stored_type.is<BaseType>());
    EXPECT_EQ(inner_slice.stored_type.get<BaseType>().type_name, "Int");
    EXPECT_TRUE(inner_slice.stored_type.get<BaseType>().instanciated_generics.empty());
}