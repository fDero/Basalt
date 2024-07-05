
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "../tests_utilities/union_definition_factory.hpp"
#include "../tests_utilities/struct_definition_factory.hpp"
#include "../tests_utilities/typesignature_factory.hpp"
#include "../tests_utilities/type_alias_factory.hpp"

TEST(Preprocessor, IntOrFloat_Is_Compatible_With_Itself) {
    ProgramRepresentation empty_program;
    empty_program.store_definitions_from_file(
        FileRepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(empty_program);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };
    bool IntOrFloat_is_compatible_with_itself = type_checker.validate_assignment(IntOrFloat, IntOrFloat);
    EXPECT_TRUE(IntOrFloat_is_compatible_with_itself);
}

TEST(Preprocessor, IntOrFloat_Is_Mutually_Compatible_With_Number) {
    ProgramRepresentation simple_union_definition_program;
    simple_union_definition_program.store_definitions_from_file(
        FileRepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = {
                UnionDefinitionFactory::make_union_definition(
                        "Number", { }, {
                                TypeSignatureFactory::Int,
                                TypeSignatureFactory::Float
                        }
                )
            },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(simple_union_definition_program);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };
    TypeSignature Number = CustomType { Token { "Number", "main.basalt", 1, 1, 1, Token::Type::type }, { } };
    bool Number_is_compatible_with_IntOrFloat = type_checker.validate_assignment(Number, IntOrFloat);
    bool IntOrFloat_is_compatible_with_Number = type_checker.validate_assignment(IntOrFloat, Number);
    EXPECT_TRUE(IntOrFloat_is_compatible_with_Number);
    EXPECT_TRUE(Number_is_compatible_with_IntOrFloat);
}

TEST(Preprocessor, Int_And_Float_Are_Compatible_With_IntOrFloat) {
    ProgramRepresentation empty_program;
    empty_program.store_definitions_from_file(
        FileRepresentation {
            .file_metadata = {
                .filename = "main.basalt",
                .packagename = "testpackage",
                .imports = { }
            },
            .type_defs = { },
            .func_defs = { }
        }
    );
    AssignmentTypeChecker type_checker(empty_program);
    TypeSignature IntOrFloat = InlineUnion { Token { "Int", "main.basalt", 1, 1, 1, Token::Type::type }, {
        TypeSignatureFactory::Int, TypeSignatureFactory::Float
    } };
    bool Int_is_compatible_with_IntOrFloat = type_checker.validate_assignment(TypeSignatureFactory::Int, IntOrFloat);
    bool Float_is_compatible_with_IntOrFloat = type_checker.validate_assignment(TypeSignatureFactory::Float, IntOrFloat);
    EXPECT_TRUE(Int_is_compatible_with_IntOrFloat);
    EXPECT_TRUE(Float_is_compatible_with_IntOrFloat);
}