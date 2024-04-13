
#include <gtest/gtest.h>
#include "toolchain/generics.hpp"
#include "errors/internal_errors.hpp"

TEST(TypeSystem, Inferred_Generics_Basetypes_Pointer) {
    TypeSignature template_type = BaseType { "T", {} };
    TypeSignature concrete_type = PointerType { BaseType { "Int", {} }};
    std::vector<std::string> generic_names { "T", "U" };
    auto res = apply_generics_to_typesignature(template_type, concrete_type, generic_names);
    ASSERT_TRUE(res.has_value());
    ASSERT_TRUE(res->size() == 1);
    GenericSubstitutionRule rule = res->back();
    ASSERT_TRUE(rule.replacement.is<PointerType>());
    ASSERT_TRUE(rule.to_be_substituded.is<BaseType>());
    ASSERT_TRUE(rule.replacement.get<PointerType>().pointed_type.is<BaseType>());
    EXPECT_EQ(rule.replacement.get<PointerType>().pointed_type.get<BaseType>().type_name, "Int");
    EXPECT_EQ(rule.to_be_substituded.get<BaseType>().type_name, "T");
}

TEST(TypeSystem, Inferred_Generics_Basetypes_Slice) {
    TypeSignature template_type = BaseType { "T", {} };
    TypeSignature concrete_type = SliceType { BaseType { "Int", {} }};
    std::vector<std::string> generic_names { "T", "U" };
    auto res = apply_generics_to_typesignature(template_type, concrete_type, generic_names);
    ASSERT_TRUE(res.has_value());
    ASSERT_TRUE(res->size() == 1);
    GenericSubstitutionRule rule = res->back();
    ASSERT_TRUE(rule.replacement.is<SliceType>());
    ASSERT_TRUE(rule.to_be_substituded.is<BaseType>());
    ASSERT_TRUE(rule.replacement.get<SliceType>().stored_type.is<BaseType>());
    EXPECT_EQ(rule.replacement.get<SliceType>().stored_type.get<BaseType>().type_name, "Int");
    EXPECT_EQ(rule.to_be_substituded.get<BaseType>().type_name, "T");
}

TEST(TypeSystem, Inferred_Generics_Basetypes_Array) {
    TypeSignature template_type = BaseType { "T", {} };
    TypeSignature concrete_type = ArrayType { 10, BaseType { "Int", {} }};
    std::vector<std::string> generic_names { "T", "U" };
    auto res = apply_generics_to_typesignature(template_type, concrete_type, generic_names);
    ASSERT_TRUE(res.has_value());
    ASSERT_TRUE(res->size() == 1);
    GenericSubstitutionRule rule = res->back();
    ASSERT_TRUE(rule.replacement.is<ArrayType>());
    ASSERT_TRUE(rule.to_be_substituded.is<BaseType>());
    ASSERT_TRUE(rule.replacement.get<ArrayType>().stored_type.is<BaseType>());
    EXPECT_EQ(rule.replacement.get<ArrayType>().stored_type.get<BaseType>().type_name, "Int");
    EXPECT_EQ(rule.to_be_substituded.get<BaseType>().type_name, "T");
}