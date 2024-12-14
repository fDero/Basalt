
#include <gtest/gtest.h>
#include "preprocessing/function_definitions_typechecker.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"

TEST(Preprocessing, FunctionDefinitionsTypeChecker_With_Assignment_Of_Array_Literal_Of_Explicit_Size) {
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
                    "f", "test.basalt", {}, {}, std::nullopt, {
                        VariableDeclaration {
                            "arr", TypeSignatureFactory::ArrayOfInts,
                            ArrayLiteral {
                                10, 
                                TypeSignatureFactory::Int, {
                                    IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "2", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "3", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "4", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "5", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "6", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "7", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "8", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "9", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
                                },
                                Token { "[", "test.basalt", 1, 1, 4, Token::Type::type }
                            },
                            Token { "x", "test.basalt", 1, 1, 1, Token::Type::text }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionDefinitionsTypeChecker type_checker(single_func_def_program);
    type_checker.visit_all_function_definitions();
}

TEST(Preprocessing, FunctionDefinitionsTypeChecker_With_Assignment_Of_Array_Literal_Of_Inferred_Size) {
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
                    "f", "test.basalt", {}, {}, std::nullopt, {
                        VariableDeclaration {
                            "arr", TypeSignatureFactory::ArrayOfInts,
                            ArrayLiteral {
                                std::nullopt, 
                                TypeSignatureFactory::Int, {
                                    IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "2", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "3", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "4", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "5", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "6", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "7", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "8", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "9", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
                                },
                                Token { "[", "test.basalt", 1, 1, 4, Token::Type::type }
                            },
                            Token { "x", "test.basalt", 1, 1, 1, Token::Type::text }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionDefinitionsTypeChecker type_checker(single_func_def_program);
    type_checker.visit_all_function_definitions();
}

TEST(Preprocessing, FunctionDefinitionsTypeChecker_With_Assignment_Of_Array_Literal_Of_Wrong_Size) {
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
                    "f", "test.basalt", {}, {}, std::nullopt, {
                        VariableDeclaration {
                            "arr", TypeSignatureFactory::ArrayOfInts,
                            ArrayLiteral {
                                10, 
                                TypeSignatureFactory::Int, {
                                    IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "2", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
                                },
                                Token { "[", "test.basalt", 1, 1, 4, Token::Type::type }
                            },
                            Token { "x", "test.basalt", 1, 1, 1, Token::Type::text }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionDefinitionsTypeChecker type_checker(single_func_def_program);
    EXPECT_ANY_THROW(type_checker.visit_all_function_definitions());
}

TEST(Preprocessing, FunctionDefinitionsTypeChecker_With_Assignment_Of_Array_Literal_Of_Incompatible_Size) {
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
                    "f", "test.basalt", {}, {}, std::nullopt, {
                        VariableDeclaration {
                            "arr", 
                            ArrayType { 
                                Token { "[", "test.basalt", 1, 1, 4, Token::Type::type}, 
                                10, 
                                TypeSignatureFactory::Int 
                            },
                            ArrayLiteral {
                                std::nullopt, 
                                TypeSignatureFactory::Int, {
                                    IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                    IntLiteral { Token { "2", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
                                },
                                Token { "[", "test.basalt", 1, 1, 4, Token::Type::type }
                            },
                            Token { "x", "test.basalt", 1, 1, 1, Token::Type::text }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionDefinitionsTypeChecker type_checker(single_func_def_program);
    EXPECT_ANY_THROW(type_checker.visit_all_function_definitions());
}