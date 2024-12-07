
#include <gtest/gtest.h>
#include "preprocessing/function_exit_path_navigator.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"

TEST(Preprocessing, Function_Exit_Path_Navigator_Stuff_After_Return_Is_Unreachable) {
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
                        Return {
                            BinaryOperator {
                                Token { "+", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                                Identifier { Token { "y", "test.basalt", 1, 1, 1, Token::Type::text } }
                            },
                            Token { "return", "test.basalt", 1, 1, 6, Token::Type::return_keyword }
                        },
                        Assignment {
                            Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                            BinaryOperator {
                                Token { "+", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                                Identifier { Token { "y", "test.basalt", 1, 1, 1, Token::Type::text } }
                            },
                            Token { "=", "test.basalt", 1, 1, 6, Token::Type::symbol }
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

TEST(Preprocessing, Function_Exit_Path_Navigator_Conditional_Where_Both_Branches_Exit_Makes_The_Remaining_Code_Unreachable) {
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
                        UntilLoop {
                            BoolLiteral{
                                Token { "true", "test.basalt", 1, 1, 1, Token::Type::text }
                            },
                            {
                                Conditional {
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
                                    {
                                        Break {
                                            Token { "break", "test.basalt", 1, 1, 6, Token::Type::break_keyword }
                                        }
                                    },
                                    Token { "if", "test.basalt", 1, 1, 1, Token::Type::if_keyword }
                                },
                                VariableDeclaration {
                                    "z",
                                    TypeSignatureFactory::Int,
                                    std::nullopt,
                                    Token { "var", "test.basalt", 1, 1, 1, Token::Type::var_keyword }
                                }
                            },
                            Token { "until", "test.basalt", 1, 1, 1, Token::Type::until_keyword }
                        },
                        Return {
                            BinaryOperator {
                                Token { "+", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                                Identifier { Token { "y", "test.basalt", 1, 1, 1, Token::Type::text } }
                            },
                            Token { "return", "test.basalt", 1, 1, 6, Token::Type::return_keyword }
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