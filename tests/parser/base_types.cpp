
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, CustomType_WithOut_Generics) {
    std::vector<Token> typesignatures = {
        { "Person",   "test.basalt", 1, 1, 1,  Token::Type::type },
        { "Employee", "test.basalt", 1, 2, 9,  Token::Type::type },
        { "Manager",  "test.basalt", 1, 3, 17, Token::Type::type }
    };
    Parser parser = Parser(typesignatures);
    for (const auto& typesignature : typesignatures) {
        TypeSignature type = parser.parse_typesignature();
        ASSERT_TRUE(type.is<CustomType>());
        ASSERT_EQ(type.get<CustomType>().type_name, typesignature.sourcetext);
        ASSERT_TRUE(type.get<CustomType>().type_parameters.empty());
    }
}

TEST(Parsing, Custom_Type_With_One_Generic) {
    std::vector<Token> typesignature = {
        { "Wrapper", "test.basalt", 1, 1, 1, Token::Type::type },
        { "<", "test.basalt", 1, 2, 7, Token::Type::symbol     },
        { "String", "test.basalt", 1, 3, 13, Token::Type::type },
        { ">", "test.basalt", 1, 4, 14, Token::Type::symbol    }
    };
    Parser parser = Parser(typesignature);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<CustomType>());
    ASSERT_EQ(type.get<CustomType>().type_name, "Wrapper");
    ASSERT_EQ(type.get<CustomType>().type_parameters.size(), 1);
    ASSERT_TRUE(type.get<CustomType>().type_parameters[0].is<PrimitiveType>());
    EXPECT_EQ(type.get<CustomType>().type_parameters[0].get<PrimitiveType>().type_name, "String");
}

TEST(Parsing, CustomType_With_Multiple_Generic) {
    std::vector<Token> typesignature = {
        { "MultiWrapper", "test.basalt", 1, 1, 1, Token::Type::type },
        { "<", "test.basalt", 1, 2, 13, Token::Type::symbol },
        { "String", "test.basalt", 1, 3, 14, Token::Type::type },
        { ",", "test.basalt", 1, 4, 20, Token::Type::symbol },
        { "Int", "test.basalt", 1, 5, 21, Token::Type::type },
        { ",", "test.basalt", 1, 6, 24, Token::Type::symbol },
        { "Bool", "test.basalt", 1, 7, 25, Token::Type::type },
        { ">", "test.basalt", 1, 8, 29, Token::Type::symbol }
    };
    Parser parser = Parser(typesignature);
    TypeSignature type = parser.parse_typesignature();
    ASSERT_TRUE(type.is<CustomType>());
    ASSERT_EQ(type.get<CustomType>().type_parameters.size(), 3);
    ASSERT_TRUE(type.get<CustomType>().type_parameters[0].is<PrimitiveType>());
    ASSERT_TRUE(type.get<CustomType>().type_parameters[1].is<PrimitiveType>());
    ASSERT_TRUE(type.get<CustomType>().type_parameters[2].is<PrimitiveType>());
    ASSERT_EQ(type.get<CustomType>().type_parameters[0].get<PrimitiveType>().type_name, "String");
    ASSERT_EQ(type.get<CustomType>().type_parameters[1].get<PrimitiveType>().type_name, "Int");
    ASSERT_EQ(type.get<CustomType>().type_parameters[2].get<PrimitiveType>().type_name, "Bool");
}