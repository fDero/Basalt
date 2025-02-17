
#include <gtest/gtest.h>
#include "syntax/keywords.hpp"
#include "preprocessing/immutability_constraint_validator.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"

TEST(Preprocessing, Immutability_Constraint_Validator_Assignment_Of_Literal_To_Var) {
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
                    "f", "test.basalt", {}, {}, 
                    std::nullopt,
                    {
                        VariableDeclaration {
                            "x",
                            TypeSignatureFactory::Int,
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { var_keyword, "test.basalt", 1, 1, 1, Token::Type::var_keyword }
                        },
                        Assignment {
                            Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { "=", "test.basalt", 1, 1, 1, Token::Type::symbol }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ImmutabilityConstraintValidator immutability_constraint_validator(single_func_def_program);
    immutability_constraint_validator.visit_all_function_definitions();
}

TEST(Preprocessing, Immutability_Constraint_Validator_Assignment_Of_Literal_To_Literal) {
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
                    "f", "test.basalt", {}, {}, 
                    std::nullopt,
                    {
                        Assignment {
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { "=", "test.basalt", 1, 1, 1, Token::Type::symbol }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ImmutabilityConstraintValidator immutability_constraint_validator(single_func_def_program);
    EXPECT_ANY_THROW(immutability_constraint_validator.visit_all_function_definitions());
}

TEST(Preprocessing, Immutability_Constraint_Validator_Assignment_Of_Literal_To_Const) {
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
                    "f", "test.basalt", {}, {}, 
                    std::nullopt,
                    {
                        ConstDeclaration {
                            "x",
                            TypeSignatureFactory::Int,
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { const_keyword, "test.basalt", 1, 1, 1, Token::Type::const_keyword }
                        },
                        Assignment {
                            Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { "=", "test.basalt", 1, 1, 1, Token::Type::symbol }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ImmutabilityConstraintValidator immutability_constraint_validator(single_func_def_program);
    EXPECT_ANY_THROW(immutability_constraint_validator.visit_all_function_definitions());
}

TEST(Preprocessing, Immutability_Constraint_Validator_Assignment_Of_Literal_To_Function_Call) {
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
                    "f", "test.basalt", {}, {}, 
                    TypeSignatureFactory::PointerToInt,
                    {
                        Assignment {
                            FunctionCall {
                                Token { "f", "test.basalt", 1, 1, 1, Token::Type::text },
                                "",
                                {},
                                {}
                            },
                            Identifier { Token { "null", "test.basalt", 1, 1, 1, Token::Type::text } },
                            Token { "=", "test.basalt", 1, 1, 1, Token::Type::symbol }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ImmutabilityConstraintValidator immutability_constraint_validator(single_func_def_program);
    EXPECT_ANY_THROW(immutability_constraint_validator.visit_all_function_definitions());
}