
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

ProjectFileStructure single_file_project_with_number_def({
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


TEST(Core, IntOrFloat_Is_Compatible_With_Itself) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };
    bool IntOrFloat_is_compatible_with_itself = type_checker.validate_assignment(IntOrFloat, IntOrFloat);
    EXPECT_TRUE(IntOrFloat_is_compatible_with_itself);
}

TEST(Core, IntOrFloat_Is_Mutually_Compatible_With_Number) {
    TypeDefinitionsRegister type_register(single_file_project_with_number_def); 
    AssignmentTypeChecker type_checker(type_register, single_file_project_with_number_def);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };
    TypeSignature Number = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, { } };
    bool Number_is_compatible_with_IntOrFloat = type_checker.validate_assignment(Number, IntOrFloat);
    bool IntOrFloat_is_compatible_with_Number = type_checker.validate_assignment(IntOrFloat, Number);
    EXPECT_TRUE(IntOrFloat_is_compatible_with_Number);
    EXPECT_TRUE(Number_is_compatible_with_IntOrFloat);
}

TEST(Core, Int_And_Float_Are_Compatible_With_IntOrFloat) {
    ProjectFileStructure empty_project;
    TypeDefinitionsRegister type_register(empty_project); 
    AssignmentTypeChecker type_checker(type_register, empty_project);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };
    bool Int_is_compatible_with_IntOrFloat = type_checker.validate_assignment(TypeSignatureFactory::Int, IntOrFloat);
    bool Float_is_compatible_with_IntOrFloat = type_checker.validate_assignment(TypeSignatureFactory::Float, IntOrFloat);
    EXPECT_TRUE(Int_is_compatible_with_IntOrFloat);
    EXPECT_TRUE(Float_is_compatible_with_IntOrFloat);
}