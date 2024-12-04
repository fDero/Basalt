
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

const std::string filename = "main.basalt";
const std::string packagename = "testpackage";

ProjectFileStructure single_file_project_with_multiple_union_defs({
    FileRepresentation {
        .file_metadata = {
            .filename = filename,
            .packagename = packagename,
            .imports = { }
        },
        .type_defs = { 
            UnionDefinitionFactory::make_union_definition(
                "Number", filename, { }, {
                    TypeSignatureFactory::Int,
                    TypeSignatureFactory::Float
                }
            ),
            UnionDefinitionFactory::make_union_definition(
                "Primitive", filename, { }, {
                    TypeSignatureFactory::Int,
                    TypeSignatureFactory::Float,
                    TypeSignatureFactory::Bool,
                    TypeSignatureFactory::String,
                    TypeSignatureFactory::RawString,
                    TypeSignatureFactory::Char
                }
            ),
            UnionDefinitionFactory::make_union_definition(
                "Either", filename, { "T", "U" }, {
                    TypeSignatureFactory::T,
                    TypeSignatureFactory::U
                }
            )
        },
        .func_defs = { }
    }
});


TEST(Core, Number_Union_Is_Compatible_With_Itself) {
    TypeDefinitionsRegister type_register(single_file_project_with_multiple_union_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_multiple_union_defs);
    TypeSignature number_type = CustomType { Token { "Number", filename, 1, 1, 1, Token::Type::type }, {} };
    bool number_is_compatible_with_itself = type_checker.validate_assignment(number_type, number_type);
    EXPECT_TRUE(number_is_compatible_with_itself);
}

TEST(Core, Int_Compatible_With_Number_Union) {
    TypeDefinitionsRegister type_register(single_file_project_with_multiple_union_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_multiple_union_defs);
    TypeSignature number_type = CustomType { Token { "Number", filename, 1, 1, 1, Token::Type::type }, {} };
    bool int_compatible_with_number = type_checker.validate_assignment(TypeSignatureFactory::Int, number_type);
    bool number_compatible_with_int = type_checker.validate_assignment(number_type, TypeSignatureFactory::Int);
    EXPECT_TRUE(int_compatible_with_number);
    EXPECT_FALSE(number_compatible_with_int);
}

TEST(Core, Number_Union_Compatible_With_Primitive_Union) {
    TypeDefinitionsRegister type_register(single_file_project_with_multiple_union_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_multiple_union_defs);
    TypeSignature number_type = CustomType { Token { "Number", filename, 1, 1, 1, Token::Type::type }, {} };
    TypeSignature primitive_type = CustomType { Token { "Primitive", filename, 1, 1, 1, Token::Type::type }, {} };
    bool number_compatible_with_primitive = type_checker.validate_assignment(number_type, primitive_type);
    bool primitive_compatible_with_number = type_checker.validate_assignment(primitive_type, number_type);
    EXPECT_TRUE(number_compatible_with_primitive);
    EXPECT_FALSE(primitive_compatible_with_number);
}

TEST(Core, Int_Compatible_With_Primitive_When_Int_Implicitly_Named_Via_Number) {
    TypeDefinitionsRegister type_register(single_file_project_with_multiple_union_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_multiple_union_defs);
    TypeSignature primitive_type = CustomType { Token { "Primitive", filename, 1, 1, 1, Token::Type::type }, {} };
    bool int_compatible_with_primitive = type_checker.validate_assignment(TypeSignatureFactory::Int, primitive_type);
    EXPECT_TRUE(int_compatible_with_primitive);
}

TEST(Core, Int_Compatible_With_Generic_Either_Union) {
    TypeDefinitionsRegister type_register(single_file_project_with_multiple_union_defs);
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_multiple_union_defs);
    TypeSignature either_int_or_float = CustomType { 
        Token { "Either", filename, 1, 1, 1, Token::Type::type }, {
            TypeSignatureFactory::Int,
            TypeSignatureFactory::Float
        } 
    };
    bool int_compatible_with_either_int_or_float = type_checker.validate_assignment(TypeSignatureFactory::Int, either_int_or_float);
    EXPECT_TRUE(int_compatible_with_either_int_or_float);
}