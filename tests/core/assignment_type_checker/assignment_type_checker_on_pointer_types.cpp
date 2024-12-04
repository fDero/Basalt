
#include <gtest/gtest.h>
#include "core/generics_substitution_rules.hpp"
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"

ProjectFileStructure single_file_project_with_number_defx({
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

TypeSignature PointerToNumber = PointerType {
    Token { "#", "main.basalt", 1, 1, 1, Token::Type::type }, 
    CustomType { Token { "Number", "main.basalt", 1, 2, 2, Token::Type::type }, { } } 
};

TEST(Core, Assignment_Type_Checking_For_Pointer_To_Int_Assigned_To_Pointer_To_IntOrFloat) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    EXPECT_FALSE(type_checker.validate_assignment(TypeSignatureFactory::PointerToInt, TypeSignatureFactory::PointerToIntOrFloat));
}

TEST(Core, Assignment_Type_Checking_For_Pointer_To_Int_Assigned_To_Pointer_To_Int) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::PointerToInt, TypeSignatureFactory::PointerToInt));
}

TEST(Core, Assignment_Type_Checking_For_Pointer_To_IntOrFloat_Assigned_To_Pointer_To_IntOrFloat) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::PointerToIntOrFloat, TypeSignatureFactory::PointerToIntOrFloat));
}

TEST(Core, Assignment_Type_Checking_For_Pointer_To_IntOrFloat_Assigned_To_Pointer_To_Number) {
    TypeDefinitionsRegister type_register(single_file_project_with_number_defx); 
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_number_defx);
    EXPECT_TRUE(type_checker.validate_assignment(TypeSignatureFactory::PointerToIntOrFloat, PointerToNumber));
}

TEST(Core, Assignment_Type_Checking_For_Pointer_To_Number_Assigned_To_Pointer_To_IntOrFloat) {
    TypeDefinitionsRegister type_register(single_file_project_with_number_defx); 
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_number_defx);
    EXPECT_TRUE(type_checker.validate_assignment(PointerToNumber, TypeSignatureFactory::PointerToIntOrFloat));
}