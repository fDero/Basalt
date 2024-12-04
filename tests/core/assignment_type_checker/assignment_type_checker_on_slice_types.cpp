
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure single_file_project_with_number_defy({
        FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            UnionDefinitionFactory::make_union_definition(
                "Number", "main.basalt", { }, {
                    TypeSignatureFactory::Int,
                    TypeSignatureFactory::Float
                }
            )
        },
        .func_defs = { }
    }
});

TypeSignature SliceOfNumbers = SliceType {
    Token { "$", "main.basalt", 1, 1, 1, Token::Type::type }, 
    CustomType { Token { "Number", "main.basalt", 1, 2, 2, Token::Type::type }, { } } 
};

TEST(Core, Assignment_Type_Checking_For_Slice_Of_Ints_Assigned_To_Slice_Of_IntsOrFloats) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    EXPECT_FALSE(type_checker.validate_assignment(TypeSignatureFactory::SliceOfInts, TypeSignatureFactory::SliceOfIntsOrFloats));
}

TEST(Core, Assignment_Type_Checking_For_Slice_Of_Ints_Assigned_To_Slice_Of_Ints) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::SliceOfInts, TypeSignatureFactory::SliceOfInts));
}

TEST(Core, Assignment_Type_Checking_For_Slice_Of_IntsOrFloat_Assigned_To_Slice_Of_IntsOrFloat) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::SliceOfIntsOrFloats, TypeSignatureFactory::SliceOfIntsOrFloats));
}

TEST(Core, Assignment_Type_Checking_For_Slice_Of_IntsOrFloat_Assigned_To_SliceOfNumbers) {
    TypeDefinitionsRegister type_register(single_file_project_with_number_defy); 
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_number_defy);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::SliceOfIntsOrFloats, SliceOfNumbers));
}

TEST(Core, Assignment_Type_Checking_For_SliceOfNumbers_Assigned_To_Slice_Of_IntsOrFloat) {
    TypeDefinitionsRegister type_register(single_file_project_with_number_defy); 
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_number_defy);
    EXPECT_TRUE(type_checker.validate_assignment(SliceOfNumbers, TypeSignatureFactory::SliceOfIntsOrFloats));
}