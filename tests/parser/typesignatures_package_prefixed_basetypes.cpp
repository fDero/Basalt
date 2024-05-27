
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Package_Prefixed_Concrete_Generic_CustomType){
    std::vector<Token> tokens = {
        { "collections", "test.basalt", 1, 1, 11, Token::Type::text   },
        { "::",          "test.basalt", 1, 2, 13, Token::Type::symbol },
        { "List",        "test.basalt", 1, 3, 17, Token::Type::type   },
        { "<",           "test.basalt", 1, 4, 18, Token::Type::symbol },
        { "Int",         "test.basalt", 1, 5, 21, Token::Type::type   },
        { ">",           "test.basalt", 1, 6, 22, Token::Type::symbol },
    };
    Parser parser = Parser(tokens);
    TypeSignature typesignature = parser.parse_typesignature();
    ASSERT_TRUE(typesignature.is<CustomType>());
    CustomType basetype = typesignature.get<CustomType>();
    EXPECT_EQ(basetype.package_prefix, "collections");
    EXPECT_EQ(basetype.type_name, "List");
    EXPECT_EQ(basetype.instantiation_generics.size(), 1);
    ASSERT_TRUE(basetype.instantiation_generics[0].is<PrimitiveType>());
    PrimitiveType generic = basetype.instantiation_generics[0].get<PrimitiveType>();
    EXPECT_EQ(generic.type_name, "Int");
}

TEST(Parsing, Package_Prefixed_Non_Generic_CustomType){
    std::vector<Token> tokens = {
        { "graphics",  "test.basalt", 1, 1, 11, Token::Type::text   },
        { "::",        "test.basalt", 1, 2, 13, Token::Type::symbol },
        { "Texture",   "test.basalt", 1, 3, 17, Token::Type::type   }
    };
    Parser parser = Parser(tokens);
    TypeSignature typesignature = parser.parse_typesignature();
    ASSERT_TRUE(typesignature.is<CustomType>());
    CustomType basetype = typesignature.get<CustomType>();
    EXPECT_EQ(basetype.package_prefix, "graphics");
    EXPECT_EQ(basetype.type_name, "Texture");
    EXPECT_TRUE(basetype.instantiation_generics.empty());
}