#include <optional>

#include <gtest/gtest.h>

#include "core/program_representation.hpp"
#include "core/project_file_structure.hpp"
#include "frontend/file_representation.hpp"
#include "frontend/token.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"
#include "language/statements.hpp"
#include "preprocessing/function_definitions_typechecker.hpp"
#include "syntax/keywords.hpp"
#include "../../tests_utilities/compilation_error_assertions.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"


TEST(Preprocessing, FunctionDefinitionsTypeChecker_With_Empty_Function_Definition) {
    ProjectFileStructure single_func_def_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { },
            .func_defs = { 
                FunctionDefinitionFactory::make_void_function_definition(
                    "f", "test.basalt", {}, {}
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionDefinitionsTypeChecker type_checker(single_func_def_program);
    type_checker.visit_all_function_definitions();
}

TEST(Preprocessing, FunctionDefinitionsTypeChecker_With_Correct_Return_Type) {
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
                    "f", "test.basalt", {}, {}, TypeSignatureFactory::Int, {
                        Return {
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { return_keyword, "test.basalt", 1, 1, 6, Token::Type::return_keyword }
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

TEST(Preprocessing, FunctionDefinitionsTypeChecker_With_Wrong_Return_Type) {
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
                    "f", "test.basalt", {}, {}, TypeSignatureFactory::Int, {
                        Return {
                            FloatLiteral { Token { "3.14", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { return_keyword, "test.basalt", 1, 1, 6, Token::Type::return_keyword }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionDefinitionsTypeChecker type_checker(single_func_def_program);
    EXPECT_COMPILATION_ERROR(ValidationError, "cannot return a value of type `Float` from a function that returns `Int`", type_checker.visit_all_function_definitions());
}

TEST(Preprocessing, FunctionDefinitionsTypeChecker_Ignored_Return_Value) {
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
                    "f", "test.basalt", {}, {}, TypeSignatureFactory::Int, {
                        FunctionCall {
                            Token { "f", "test.basalt", 1, 1, 1, Token::Type::text }, 
                            "", {}, {}
                        },
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionDefinitionsTypeChecker type_checker(single_func_def_program);
    EXPECT_COMPILATION_ERROR(ValidationError, "the value returned by `f` is ignored", type_checker.visit_all_function_definitions());
}

TEST(Preprocessing, FunctionDefinitionsTypeChecker_Procedure_Call) {
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
                        FunctionCall {
                            Token { "f", "test.basalt", 1, 1, 1, Token::Type::text }, 
                            "", {}, {}
                        },
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    FunctionDefinitionsTypeChecker type_checker(single_func_def_program);
    type_checker.visit_all_function_definitions();
}