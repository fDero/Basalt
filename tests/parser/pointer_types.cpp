
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Simple_Pointer) {
    std::vector<Token> pointertokens = {
        { "#", "test.basalt",   1, 1, 1, Token::Type::symbol },
        { "Int", "test.basalt", 1, 2, 2, Token::Type::type   },
    };
    Parser parser = Parser(pointertokens);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<PointerType>());
    ASSERT_TRUE(type.get<PointerType>().pointed_type.is<PrimitiveType>());
    EXPECT_EQ(type.get<PointerType>().pointed_type.get<PrimitiveType>().type_name, "Int");
}

TEST(Parsing, Pointer_To_Pointer) {
    std::vector<Token> pointertokens = {
        { "#", "test.basalt",   1, 1, 1, Token::Type::symbol },
        { "#", "test.basalt",   1, 2, 2, Token::Type::symbol },
        { "Int", "test.basalt", 1, 3, 3, Token::Type::type   },
    };
    Parser parser = Parser(pointertokens);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<PointerType>());
    ASSERT_TRUE(type.get<PointerType>().pointed_type.is<PointerType>());
    PointerType inner_pointer = type.get<PointerType>().pointed_type.get<PointerType>();
    ASSERT_TRUE(inner_pointer.pointed_type.is<PrimitiveType>());
    EXPECT_EQ(inner_pointer.pointed_type.get<PrimitiveType>().type_name, "Int");
}
