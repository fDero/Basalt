
#include <gtest/gtest.h>
#include "preprocessing/function_exit_path_navigator.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"

TEST(Preprocessing, Exit_Path_Navigator_Function_With_Return) {
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
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionExitPathNavigator exit_path_navigator(single_func_def_program);
    exit_path_navigator.visit_all_function_definitions();
}

TEST(Preprocessing, Exit_Path_Navigator_Function_With_Return_Inside_Of_A_While_But_Also_Outside) {
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
    exit_path_navigator.visit_all_function_definitions();
}

TEST(Preprocessing, Exit_Path_Navigator_Function_The_Only_Path_Returns_Inside_Of_An_Until_Hence_Function_Returns) {
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
                                Return {
                                    BinaryOperator {
                                        Token { "+", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                        Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                                        Identifier { Token { "y", "test.basalt", 1, 1, 1, Token::Type::text } }
                                    },
                                    Token { "return", "test.basalt", 1, 1, 6, Token::Type::return_keyword }
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
    exit_path_navigator.visit_all_function_definitions();
}

TEST(Preprocessing, Exit_Path_Navigator_Function_Every_Path_Returns_Inside_Of_An_Until_Hence_Function_Returns) {
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
                                        Return {
                                            BinaryOperator {
                                                Token { "+", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                                Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                                                Identifier { Token { "y", "test.basalt", 1, 1, 1, Token::Type::text } }
                                            },
                                            Token { "return", "test.basalt", 1, 1, 6, Token::Type::return_keyword }
                                        }
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
    exit_path_navigator.visit_all_function_definitions();
}

TEST(Preprocessing, Exit_Path_Navigator_Local_Control_Flow_Alteration_Doesnt_Propagate_Outside_Of_Until_Loop) {
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
    exit_path_navigator.visit_all_function_definitions();
}

TEST(Preprocessing, Exit_Path_Navigator_Function_Every_Path_Returns_Inside_Of_An_Until_Hence_Function_Returns_But_Does_Outside_If) {
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
    exit_path_navigator.visit_all_function_definitions();
}