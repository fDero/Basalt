
#include <gtest/gtest.h>
#include "preprocessing/immutability_checker.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"

TEST(Preprocessing, Immutability_Checker_Says_Square_Brackets_Access_On_Fcall_Is_NOT_Immutable_Hence_Mutable) {
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
                    "get_ints", "test.basalt", {}, {}, 
                    TypeSignatureFactory::SliceOfInts
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, single_func_def_program);
    SquareBracketsAccess sqb_access {
        Token { ".", "test.basalt", 1, 1, 1, Token::Type::symbol },
        FunctionCall {
            Token { "get_ints", "test.basalt", 1, 1, 1, Token::Type::text }, "", {}, {}
        },
        IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
    };
    EXPECT_FALSE(immutability_checker.is_strictly_immutable_expression(sqb_access));
}

TEST(Preprocessing, Immutability_Checker_Says_Square_Brackets_Access_On_Fcall_Is_Immutable) {
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
                    "get_ints", "test.basalt", {}, {}, 
                    TypeSignatureFactory::ArrayOfInts
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, single_func_def_program);
    SquareBracketsAccess sqb_access {
        Token { ".", "test.basalt", 1, 1, 1, Token::Type::symbol },
        FunctionCall {
            Token { "get_ints", "test.basalt", 1, 1, 1, Token::Type::text }, "", {}, {}
        },
        IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
    };
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(sqb_access));
}

TEST(Preprocessing, Immutability_Checker_Square_Brackets_Member_Access_On_Const_Array_Is_Immutable) {
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    ConstDeclaration const_declaration { 
        identifier.name, 
        TypeSignatureFactory::ArrayOfInts,
        ArrayLiteral { std::nullopt, TypeSignatureFactory::Int, { 
                IntLiteral { Token { "1", "test.basalt", 1, 2, 2, Token::Type::integer_literal } },
                IntLiteral { Token { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal } } 
            },
            Token { "[", "test.basalt", 1, 1, 1, Token::Type::integer_literal } 
        },
        Token { "const", "test.basalt", 1, 1, 1, Token::Type::const_keyword }
    };
    SquareBracketsAccess sqb_access {
        Token { "[", "test.basalt", 1, 1, 1, Token::Type::symbol },
        identifier,
        IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
    };
    ProjectFileStructure example_project;
    ProgramRepresentation example_program(example_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, example_program);
    scope_context.store_local_constant(const_declaration);
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(sqb_access));
}

TEST(Preprocessing, Immutability_Checker_Square_Brackets_Member_Access_On_Var_Array_Is_Immutable) {
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    VariableDeclaration var_declaration { 
        identifier.name, 
        TypeSignatureFactory::ArrayOfInts,
        ArrayLiteral { std::nullopt, TypeSignatureFactory::Int, { 
                IntLiteral { Token { "1", "test.basalt", 1, 2, 2, Token::Type::integer_literal } },
                IntLiteral { Token { "2", "test.basalt", 1, 3, 3, Token::Type::integer_literal } } 
            },
            Token { "[", "test.basalt", 1, 1, 1, Token::Type::integer_literal } 
        },
        Token { "var", "test.basalt", 1, 1, 1, Token::Type::var_keyword }
    };
    SquareBracketsAccess sqb_access {
        Token { "[", "test.basalt", 1, 1, 1, Token::Type::symbol },
        identifier,
        IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
    };
    ProjectFileStructure example_project;
    ProgramRepresentation example_program(example_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, example_program);
    scope_context.store_local_variable(var_declaration);
    EXPECT_FALSE(immutability_checker.is_strictly_immutable_expression(sqb_access));
}

TEST(Preprocessing, Immutability_Checker_Square_Brackets_Member_Access_On_Const_Slice_Is_Immutable) {
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    ConstDeclaration const_declaration { 
        identifier.name, 
        TypeSignatureFactory::SliceOfInts,
        Identifier { 
            Token { "outsorced", "test.basalt", 1, 1, 1, Token::Type::text } 
        },
        Token { "const", "test.basalt", 1, 1, 1, Token::Type::const_keyword }
    };
    SquareBracketsAccess sqb_access {
        Token { "[", "test.basalt", 1, 1, 1, Token::Type::symbol },
        identifier,
        IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
    };
    ProjectFileStructure example_project;
    ProgramRepresentation example_program(example_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, example_program);
    scope_context.store_local_constant(const_declaration);
    EXPECT_TRUE(immutability_checker.is_strictly_immutable_expression(sqb_access));
}

TEST(Preprocessing, Immutability_Checker_Square_Brackets_Member_Access_On_Var_Slice_Is_NOT_Immutable_Hence_Mutable) {
    Identifier identifier { 
        Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } 
    };
    VariableDeclaration var_declaration { 
        identifier.name, 
        TypeSignatureFactory::SliceOfInts,
        std::nullopt,
        Token { "var", "test.basalt", 1, 1, 1, Token::Type::var_keyword }
    };
    SquareBracketsAccess sqb_access {
        Token { "[", "test.basalt", 1, 1, 1, Token::Type::symbol },
        identifier,
        IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
    };
    ProjectFileStructure example_project;
    ProgramRepresentation example_program(example_project);
    ScopeContext scope_context;
    ImmutabilityChecker immutability_checker(scope_context, example_program);
    scope_context.store_local_variable(var_declaration);
    EXPECT_FALSE(immutability_checker.is_strictly_immutable_expression(sqb_access));
}