
#include <gtest/gtest.h>
#include "toolchain/generics.hpp"
#include "errors/internal_errors.hpp"

TEST(TypeSystem, Trivial_Generics_Instanciation_BaseType) {
    TypeSignature template_type = BaseType { "T", {} };
    TypeSignature concrete_type = BaseType { "Int", {} };
    std::vector<std::string> generic_names { "T" };
    auto res = apply_generics_to_typesignature(template_type, concrete_type, generic_names);
    ASSERT_TRUE(res.has_value());
    ASSERT_TRUE(res->size() == 1);
    GenericSubstitutionRule rule = res->back();
    ASSERT_TRUE(rule.replacement.is<BaseType>());
    ASSERT_TRUE(rule.to_be_substituded.is<BaseType>());
    EXPECT_EQ(rule.replacement.get<BaseType>().type_name, "Int");
    EXPECT_EQ(rule.to_be_substituded.get<BaseType>().type_name, "T");
}

TEST(TypeSystem, Trivial_Generics_Instanciation_Pair_Struct_Compatible) {
    TypeSignature template_type = BaseType { "Pair", { BaseType { "T", {} }, BaseType { "U", {} }}};
    TypeSignature concrete_type = BaseType { "Pair", { BaseType { "Int", {} }, BaseType { "String", {}}}};
    std::vector<std::string> generic_names { "T", "U" };
    auto res = apply_generics_to_typesignature(template_type, concrete_type, generic_names);
    ASSERT_TRUE(res.has_value());
    ASSERT_TRUE(res->size() == 2);
    GenericSubstitutionRule Trule = (*(res))[0];
    GenericSubstitutionRule Urule = (*(res))[1];
    ASSERT_TRUE(Trule.replacement.is<BaseType>());
    ASSERT_TRUE(Trule.to_be_substituded.is<BaseType>());
    ASSERT_TRUE(Urule.replacement.is<BaseType>());
    ASSERT_TRUE(Urule.to_be_substituded.is<BaseType>());
    EXPECT_EQ(Trule.to_be_substituded.get<BaseType>().type_name, "T");
    EXPECT_EQ(Trule.replacement.get<BaseType>().type_name, "Int");
    EXPECT_EQ(Urule.replacement.get<BaseType>().type_name, "String");
    EXPECT_EQ(Urule.to_be_substituded.get<BaseType>().type_name, "U");
}

TEST(TypeSystem, Trivial_Generics_Instanciation_Pair_Struct_Partially_Incompatible) {
    TypeSignature template_type = BaseType { "Pair", { BaseType { "T", {} }, BaseType { "Int", {} }}};
    TypeSignature concrete_type = BaseType { "Pair", { BaseType { "Int", {} }, BaseType { "String", {}}}};
    std::vector<std::string> generic_names { "T", "U" };
    auto res = apply_generics_to_typesignature(template_type, concrete_type, generic_names);
    ASSERT_FALSE(res.has_value());
}