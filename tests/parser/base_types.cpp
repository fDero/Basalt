
#include <gtest/gtest.h>
#include "toolchain/parser.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TEST(Parsing, Base_Type_WithOut_Generics) {
    std::vector<Token> typesignatures = {
        { "T",        "test.basalt", 1, 1, 1,  Token::Type::type },
        { "Typename", "test.basalt", 1, 2, 9,  Token::Type::type },
        { "TypeName", "test.basalt", 1, 3, 17, Token::Type::type }
    };
    Parser parser = Parser(typesignatures);
    for (const auto& typesignature : typesignatures){
        TypeSignature type = parser.parse_typesignature();
        ASSERT_TRUE(type.is<BaseType>());
        ASSERT_EQ(type.get<BaseType>().type_name, typesignature.sourcetext);
        ASSERT_TRUE(type.get<BaseType>().instanciated_generics.empty());
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
    ASSERT_TRUE(type.is<BaseType>());
    ASSERT_EQ(type.get<BaseType>().type_name, "Wrapper");
    ASSERT_EQ(type.get<BaseType>().instanciated_generics.size(), 1);
    ASSERT_TRUE(type.get<BaseType>().instanciated_generics[0].is<BaseType>());
    EXPECT_EQ(type.get<BaseType>().instanciated_generics[0].get<BaseType>().type_name, "String");
}

TEST(Parsing, Base_Type_With_Multiple_Generic) {
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
    ASSERT_TRUE(type.is<BaseType>());
    ASSERT_EQ(type.get<BaseType>().instanciated_generics.size(), 3);
    ASSERT_TRUE(type.get<BaseType>().instanciated_generics[0].is<BaseType>());
    ASSERT_TRUE(type.get<BaseType>().instanciated_generics[1].is<BaseType>());
    ASSERT_TRUE(type.get<BaseType>().instanciated_generics[2].is<BaseType>());
    ASSERT_EQ(type.get<BaseType>().instanciated_generics[0].get<BaseType>().type_name, "String");
    ASSERT_EQ(type.get<BaseType>().instanciated_generics[1].get<BaseType>().type_name, "Int");
    ASSERT_EQ(type.get<BaseType>().instanciated_generics[2].get<BaseType>().type_name, "Bool");
}