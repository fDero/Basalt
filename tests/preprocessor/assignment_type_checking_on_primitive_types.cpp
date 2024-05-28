
#include <gtest/gtest.h>
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"

TEST(Preprocessor, Assignment_Type_Checking_For_Int_Assigned_To_Int) {
    ProgramRepresentation empty_program; 
    AssignmentTypeChecker type_checker(empty_program);
    bool int_is_compatible_with_int = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::Int);
    EXPECT_TRUE(int_is_compatible_with_int);
}

TEST(Preprocessor, Assignment_Type_Checking_For_Float_Assigned_To_Int) {
    ProgramRepresentation empty_program; 
    AssignmentTypeChecker type_checker(empty_program);
    bool float_is_compatible_with_int = type_checker.validate_assignment(TypeSignatureFactory::Float, TypeSignatureFactory::Int);
    EXPECT_FALSE(float_is_compatible_with_int);
}

TEST(Preprocessor, Assignment_Type_Checking_For_Bool_Assigned_To_Float) {
    ProgramRepresentation empty_program; 
    AssignmentTypeChecker type_checker(empty_program);
    bool bool_is_compatible_with_float = type_checker.validate_assignment(TypeSignatureFactory::Bool, TypeSignatureFactory::Float);
    EXPECT_FALSE(bool_is_compatible_with_float);
}

TEST(Preprocessor, Assignment_Type_Checking_For_String_Assigned_To_RawSring) {
    ProgramRepresentation empty_program; 
    AssignmentTypeChecker type_checker(empty_program);
    bool string_is_compatible_with_raw_string = type_checker.validate_assignment(TypeSignatureFactory::String, TypeSignatureFactory::RawString);
    EXPECT_TRUE(string_is_compatible_with_raw_string);
}

TEST(Preprocessor, Assignment_Type_Checking_For_Char_Slice_Assigned_To_String) {
    ProgramRepresentation empty_program; 
    AssignmentTypeChecker type_checker(empty_program);
    bool char_slice_is_compatible_with_string = type_checker.validate_assignment(TypeSignatureFactory::SliceOfChars, TypeSignatureFactory::String);
    EXPECT_TRUE(char_slice_is_compatible_with_string);
}

TEST(Preprocessor, Assignment_Type_Checking_For_String_Assigned_To_RawString) {
    ProgramRepresentation empty_program; 
    AssignmentTypeChecker type_checker(empty_program);
    bool string_is_compatible_with_rawstring = type_checker.validate_assignment(TypeSignatureFactory::String, TypeSignatureFactory::RawString);
    EXPECT_TRUE(string_is_compatible_with_rawstring);
}

TEST(Preprocessor, Assignment_Type_Checking_For_RawString_Assigned_To_String) {
    ProgramRepresentation empty_program; 
    AssignmentTypeChecker type_checker(empty_program);
    bool rawstring_is_compatible_with_string = type_checker.validate_assignment(TypeSignatureFactory::RawString, TypeSignatureFactory::String);
    EXPECT_FALSE(rawstring_is_compatible_with_string);
}