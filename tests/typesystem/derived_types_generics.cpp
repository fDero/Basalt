
#include <gtest/gtest.h>
#include "toolchain/generics.hpp"
#include "errors/internal_errors.hpp"

TEST(TypeSystem, Trivial_Generics_Instanciation_Array_Of_Same_Sizes) {
    TypeSignature template_type = ArrayType { 10, BaseType { "T", {} } };
    TypeSignature concrete_type = ArrayType { 10, BaseType { "Int", {} } };
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

TEST(TypeSystem, Trivial_Generics_Instanciation_Array_Of_Different_Sizes) {
    TypeSignature template_type = ArrayType { 10, BaseType { "T", {} } };
    TypeSignature concrete_type = ArrayType { 5, BaseType { "Int", {} } };
    std::vector<std::string> generic_names { "T" };
    auto res = apply_generics_to_typesignature(template_type, concrete_type, generic_names);
    ASSERT_FALSE(res.has_value());
}

TEST(TypeSystem, Trivial_Generics_Instanciation_Slice) {
    TypeSignature template_type = SliceType { BaseType { "T", {} } };
    TypeSignature concrete_type = SliceType { BaseType { "Int", {} } };
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

TEST(TypeSystem, Trivial_Generics_Instanciation_Pointer) {
    TypeSignature template_type = PointerType { BaseType { "T", {} } };
    TypeSignature concrete_type = PointerType { BaseType { "Int", {} } };
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