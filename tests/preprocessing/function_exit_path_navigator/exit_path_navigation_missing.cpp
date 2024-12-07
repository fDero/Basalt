
#include <gtest/gtest.h>
#include "preprocessing/function_exit_path_navigator.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"

TEST(Preprocessing, Exit_Path_Navigator_Function_With_No_Return) {
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
    FunctionExitPathNavigator exit_path_navigator(single_func_def_program);
    EXPECT_ANY_THROW(exit_path_navigator.visit_all_function_definitions());
}

TEST(Preprocessing, Exit_Path_Navigator_Function_With_Return_Inside_Of_A_While_Is_Not_Propagated_Because_While_Execution_Is_Not_Guaranteed) {
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
                    TypeSignatureFactory::Int,
                    {
                        WhileLoop {
                            BoolLiteral{
                                Token { "true", "test.basalt", 1, 1, 1, Token::Type::text }
                            },
                            {
                                Return {
                                    BinaryOperator {
                                        Token { "+", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                        Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                                        Identifier { Token { "y", "test.basalt", 1, 1, 1, Token::Type::text } }
                                    },
                                    Token { "return", "test.basalt", 1, 1, 6, Token::Type::return_keyword }
                                }
                            },
                            Token { "while", "test.basalt", 1, 1, 1, Token::Type::while_keyword }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionExitPathNavigator exit_path_navigator(single_func_def_program);
    EXPECT_ANY_THROW(exit_path_navigator.visit_all_function_definitions());
}