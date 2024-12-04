
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

TEST(Core, Type_Inference_Algorithm_Finds_SuperType_Of_Int_And_Float_As_Their_Inline_Union) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project);
    AssignmentTypeChecker type_checker(type_register, empty_project);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T));
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Float, TypeSignatureFactory::T));
    ASSERT_EQ(type_checker.get_generic_substitution_rules()->size(), 1);
    EXPECT_TRUE(type_checker.get_generic_substitution_rules()->back().replacement.is<InlineUnion>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->back().replacement.get<InlineUnion>().alternatives.size(), 2);
}

TEST(Core, Type_Inference_Algorithm_Finds_SuperType_Of_Int_Float_And_Float_As_Their_Inline_Union) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project);
    AssignmentTypeChecker type_checker(type_register, empty_project);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T));
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Float, TypeSignatureFactory::T));
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::String, TypeSignatureFactory::T));
    ASSERT_EQ(type_checker.get_generic_substitution_rules()->size(), 1);
    EXPECT_TRUE(type_checker.get_generic_substitution_rules()->back().replacement.is<InlineUnion>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->back().replacement.get<InlineUnion>().alternatives.size(), 3);
}

TEST(Core, Type_Inference_Algorithm_Finds_SuperType_Of_IntOrFloat_And_String_As_IntOrFloatOrString) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project);
    AssignmentTypeChecker type_checker(type_register, empty_project);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };
    EXPECT_TRUE(type_checker.validate_assignment(IntOrFloat, TypeSignatureFactory::T));
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::String, TypeSignatureFactory::T));
    ASSERT_EQ(type_checker.get_generic_substitution_rules()->size(), 1);
    EXPECT_TRUE(type_checker.get_generic_substitution_rules()->back().replacement.is<InlineUnion>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules()->back().replacement.get<InlineUnion>().alternatives.size(), 3);
}