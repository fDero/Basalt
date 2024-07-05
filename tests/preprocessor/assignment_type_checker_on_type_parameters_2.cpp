
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"

TEST(Preprocessor, Inferred_Generics_Do_Not_Allow_Too_Much_Flexibility_On_Type_Parameters) {
    ProgramRepresentation empty_program;
    empty_program.store_definitions_from_file(
        FileRepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "List", { "T" }, { StructDefinitionFactory::no_fields }
                ),
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(empty_program);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };

    TypeSignature list_of_T = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } };
    TypeSignature list_of_IntOrFloat = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { IntOrFloat } };

    bool Int_is_compatible_with_T = type_checker.validate_assignment(TypeSignatureFactory::Int, TypeSignatureFactory::T);
    bool List_Of_IntOrFloat_is_compatible_with_list_of_T = type_checker.validate_assignment(list_of_IntOrFloat, list_of_T);
    EXPECT_TRUE(Int_is_compatible_with_T);
    EXPECT_FALSE(List_Of_IntOrFloat_is_compatible_with_list_of_T);
}

TEST(Preprocessor, Inferred_Generics_Do_Not_Allow_Too_Much_Flexibility_On_Type_Parameters_Inverse) {
    ProgramRepresentation empty_program;
    empty_program.store_definitions_from_file(
        FileRepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "List", { "T" }, { StructDefinitionFactory::no_fields }
                ),
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(empty_program);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };

    TypeSignature list_of_T = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::T } };
    TypeSignature list_of_Int = CustomType { Token { "List", "main.basalt", 1, 1, 1, Token::Type::type }, { TypeSignatureFactory::Int } };

    bool IntOrFloat_is_compatible_with_T = type_checker.validate_assignment(IntOrFloat, TypeSignatureFactory::T);
    bool List_Of_IntOrFloat_is_compatible_with_list_of_T = type_checker.validate_assignment(list_of_Int, list_of_T);
    EXPECT_TRUE(IntOrFloat_is_compatible_with_T);
    EXPECT_FALSE(List_Of_IntOrFloat_is_compatible_with_list_of_T);
}