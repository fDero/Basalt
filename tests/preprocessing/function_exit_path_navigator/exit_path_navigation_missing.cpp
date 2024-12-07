
#include <gtest/gtest.h>
#include "preprocessing/function_exit_path_navigator.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"

TEST(Preprocessing, Function_Exit_Path_Navigator_Missing_Return) {
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

TEST(Preprocessing, Function_Exit_Path_Navigator_Return_Inside_Of_A_While_Does_Not_Count_Because_While_Execution_Is_Not_Guaranteed) {
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

TEST(Preprocessing, Function_Exit_Path_Navigator_Not_Every_Path_Returns_Inside_Of_An_Until_Hence_Function_Does_Not_Return) {
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
                                    },
                                    Token { "if", "test.basalt", 1, 1, 1, Token::Type::if_keyword }
                                }
                            },
                            Token { "until", "test.basalt", 1, 1, 1, Token::Type::until_keyword }
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

TEST(Preprocessing, Function_Exit_Path_Navigator_Local_Control_Flow_Alteration_Doesnt_Count_As_Definitive_Exit) {
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
                                Break {
                                    Token { "break", "test.basalt", 1, 1, 6, Token::Type::break_keyword }
                                }
                            },
                            Token { "until", "test.basalt", 1, 1, 1, Token::Type::until_keyword }
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