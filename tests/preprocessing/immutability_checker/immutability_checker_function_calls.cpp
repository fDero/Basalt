
#include <gtest/gtest.h>
#include "preprocessing/immutability_checker.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"

TEST(Preprocessing, Immutability_Checker_Says_Function_Call_Output_Is_Immutable) {
    ProjectFileStructure single_func_def_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { },
            .func_defs = { 
                FunctionDefinitionFactory::make_function_definition(
                    "add", "test.basalt", {}, { 
                        FunctionDefinition::Argument { 
                            "x", TypeSignatureFactory::Int 
                        },
                        FunctionDefinition::Argument { 
                            "y", TypeSignatureFactory::Int 
                        }
                    }, 
                    TypeSignatureFactory::Int
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, single_func_def_program);
    FunctionCall func_call {
        Token { "add", "test.basalt", 1, 1, 1, Token::Type::text }, "",
        { 
            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
            IntLiteral { Token { "2", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
        },
        {}
    };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(func_call));
    EXPECT_TRUE(immutability_checker.is_weakly_immutable_expression(func_call));
}

TEST(Preprocessing, Immutability_Checker_Says_Function_Call_Output_Is_Weakly_Immutable_If_Pointer) {
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
    FunctionCall func_call {
        Token { "get_int_wrapper_ptr", "test.basalt", 1, 1, 1, Token::Type::text }, "", {}, {}
    };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(func_call));
    EXPECT_FALSE(immutability_checker.is_weakly_immutable_expression(func_call));
}