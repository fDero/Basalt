
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Type_Inference_Algorithm_Finds_SuperType_Of_Int_And_Float_As_Their_Inline_Union) {
    ProgramRepresentation empty_program;
    AssignmentTypeChecker type_checker(empty_program);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T));
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Float, TypeSignatureFactory::T));
    ASSERT_EQ(type_checker.get_generic_substitution_rules().size(), 1);
    EXPECT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<InlineUnion>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<InlineUnion>().alternatives.size(), 2);
}

TEST(Preprocessor, Type_Inference_Algorithm_Finds_SuperType_Of_Int_Float_And_Float_As_Their_Inline_Union) {
    ProgramRepresentation empty_program;
    AssignmentTypeChecker type_checker(empty_program);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T));
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::Float, TypeSignatureFactory::T));
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::String, TypeSignatureFactory::T));
    ASSERT_EQ(type_checker.get_generic_substitution_rules().size(), 1);
    EXPECT_TRUE(type_checker.get_generic_substitution_rules().back().replacement.is<InlineUnion>());
    EXPECT_EQ(type_checker.get_generic_substitution_rules().back().replacement.get<InlineUnion>().alternatives.size(), 3);
}