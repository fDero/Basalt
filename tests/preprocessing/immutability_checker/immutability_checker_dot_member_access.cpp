
#include <gtest/gtest.h>
#include "preprocessing/immutability_checker.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"

TEST(Preprocessing, Immutability_Checker_Says_Dot_Member_Access_On_Fcall_Is_NOT_Immutable_Hence_Mutable) {
    ProjectFileStructure single_func_def_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "IntWrapper", "test.basalt", {}, {
                        StructDefinition::Field { "wrapped", TypeSignatureFactory::Int }
                    }                
                )
            },
            .func_defs = { 
                FunctionDefinitionFactory::make_function_definition(
                    "get_int_wrapper_ptr", "test.basalt", {}, {}, 
                    PointerType {
                        Token { "#", "test.basalt", 1, 1, 1, Token::Type::type },
                        CustomType {
                            Token { "IntWrapper", "test.basalt", 1, 1, 1, Token::Type::type },
                            {}
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, single_func_def_program);
    DotMemberAccess dot_member_access {
        Token { ".", "test.basalt", 1, 1, 1, Token::Type::symbol },
        FunctionCall {
            Token { "get_int_wrapper_ptr", "test.basalt", 1, 1, 1, Token::Type::text }, "", {}, {}
        },
        "wrapped"
    };
    EXPECT_FALSE(immutability_checker.is_strictly_immutable_expression(dot_member_access));
}

TEST(Preprocessing, Immutability_Checker_Says_Dot_Member_Access_On_Variable_Is_NOT_Immutable_Hence_Mutable) {
    ProjectFileStructure example_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "IntWrapper", "test.basalt", {}, {
                        StructDefinition::Field { "wrapped", TypeSignatureFactory::Int }
                    }                
                )
            },
            .func_defs = { }
        }
    });
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    VariableDeclaration var_declaration { 
        identifier.name, 
        CustomType { Token { "IntWrapper", "test.basalt", 1, 1, 1, Token::Type::type }, {} },
        std::nullopt,
        Token { "var", "test.basalt", 1, 1, 1, Token::Type::var_keyword }
    };
    DotMemberAccess dot_member_access {
        Token { ".", "test.basalt", 1, 1, 1, Token::Type::symbol },
        identifier,
        "wrapped"
    };
    ProgramRepresentation example_program(example_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, example_program);
    scope_context.store_local_variable(var_declaration);
    EXPECT_FALSE(immutability_checker.is_strictly_immutable_expression(dot_member_access));
}

TEST(Preprocessing, Immutability_Checker_Says_Dot_Member_Access_On_Const_Is_Immutable) {
    ProjectFileStructure example_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "IntWrapper", "test.basalt", {}, {
                        StructDefinition::Field { "wrapped", TypeSignatureFactory::Int }
                    }                
                )
            },
            .func_defs = { }
        }
    });
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    ConstDeclaration const_declaration { 
        identifier.name, 
        CustomType { Token { "IntWrapper", "test.basalt", 1, 1, 1, Token::Type::type }, {} },
        Identifier { Token { "outsorced", "test.basalt", 1, 1, 1, Token::Type::text } },
        Token { "const", "test.basalt", 1, 1, 1, Token::Type::const_keyword }
    };
    DotMemberAccess dot_member_access {
        Token { ".", "test.basalt", 1, 1, 1, Token::Type::symbol },
        identifier,
        "wrapped"
    };
    ProgramRepresentation example_program(example_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, example_program);
    scope_context.store_local_constant(const_declaration);
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(dot_member_access));
}