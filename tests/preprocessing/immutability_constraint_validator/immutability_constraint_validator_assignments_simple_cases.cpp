#include <optional>

#include <gtest/gtest.h>

#include "core/program_representation.hpp"
#include "core/project_file_structure.hpp"
#include "frontend/file_representation.hpp"
#include "frontend/token.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"
#include "language/statements.hpp"
#include "preprocessing/immutability_constraint_validator.hpp"
#include "syntax/keywords.hpp"
#include "../../tests_utilities/compilation_error_assertions.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"


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
    EXPECT_COMPILATION_ERROR(ValidationError, "cannot assign to this expression: it is immutable", immutability_constraint_validator.visit_all_function_definitions());
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
    EXPECT_COMPILATION_ERROR(ValidationError, "cannot assign to `x` because it is a constant", immutability_constraint_validator.visit_all_function_definitions());
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
    EXPECT_COMPILATION_ERROR(ValidationError, "cannot assign to this expression", immutability_constraint_validator.visit_all_function_definitions());
}