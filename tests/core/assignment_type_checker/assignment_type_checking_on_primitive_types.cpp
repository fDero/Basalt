
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

TEST(Core, Assignment_Type_Checking_For_Int_Assigned_To_Int) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    bool int_is_compatible_with_int = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::Int);
    EXPECT_TRUE(int_is_compatible_with_int);
}

TEST(Core, Assignment_Type_Checking_For_Float_Assigned_To_Int) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    bool float_is_compatible_with_int = type_checker.validate_assignment(TypeSignatureFactory::Float, TypeSignatureFactory::Int);
    EXPECT_FALSE(float_is_compatible_with_int);
}

TEST(Core, Assignment_Type_Checking_For_Bool_Assigned_To_Float) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    bool bool_is_compatible_with_float = type_checker.validate_assignment(TypeSignatureFactory::Bool, TypeSignatureFactory::Float);
    EXPECT_FALSE(bool_is_compatible_with_float);
}

TEST(Core, Assignment_Type_Checking_For_Char_Slice_Assigned_To_String) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    bool char_slice_is_compatible_with_string = type_checker.validate_assignment(TypeSignatureFactory::SliceOfChars, TypeSignatureFactory::String);
    EXPECT_TRUE(char_slice_is_compatible_with_string);
}

TEST(Core, Assignment_Type_Checking_For_String_Assigned_To_RawString) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    bool string_is_compatible_with_rawstring = type_checker.validate_assignment(TypeSignatureFactory::String, TypeSignatureFactory::RawString);
    EXPECT_TRUE(string_is_compatible_with_rawstring);
}

TEST(Core, Assignment_Type_Checking_For_RawString_Assigned_To_String) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    bool rawstring_is_compatible_with_string = type_checker.validate_assignment(TypeSignatureFactory::RawString, TypeSignatureFactory::String);
    EXPECT_FALSE(rawstring_is_compatible_with_string);
}