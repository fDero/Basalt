
#include <gtest/gtest.h>
#include "syntax/keywords.hpp"
#include "preprocessing/immutability_constraint_validator.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"

TEST(Preprocessing, Immutability_Constraint_Validator_Assignment_Of_Address_Of_Const_To_Var) {
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
                            "v",
                            TypeSignatureFactory::PointerToInt,
                            std::nullopt,
                            Token { var_keyword, "test.basalt", 1, 1, 1, Token::Type::var_keyword }
                        },
                        ConstDeclaration {
                            "x",
                            TypeSignatureFactory::Int,
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { const_keyword, "test.basalt", 1, 1, 1, Token::Type::const_keyword }
                        },
                        Assignment {
                            Identifier { Token { "v", "test.basalt", 1, 1, 1, Token::Type::text } },
                            UnaryOperator {
                                Token { "&", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } }
                            },
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

TEST(Preprocessing, Immutability_Constraint_Validator_Assignment_Of_Const_Struct_To_Var_Struct_Where_Structs_Have_Pointers) {
    ProjectFileStructure single_func_def_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "IntPtrWrapper", "test.basalt", {}, { 
                        StructDefinition::Field { 
                            "wrapped", TypeSignatureFactory::PointerToInt 
                        }
                    }
                )
            },
            .func_defs = { 
                FunctionDefinitionFactory::make_function_definition(
                    "f", "test.basalt", {}, {}, 
                    std::nullopt,
                    {
                        VariableDeclaration {
                            "v",
                            CustomType {
                                Token { "IntPtrWrapper", "test.basalt", 1, 1, 1, Token::Type::type },
                                { }
                            },
                            std::nullopt,
                            Token { var_keyword, "test.basalt", 1, 1, 1, Token::Type::var_keyword }
                        },
                        ConstDeclaration {
                            "x",
                            CustomType {
                                Token { "IntPtrWrapper", "test.basalt", 1, 1, 1, Token::Type::type },
                                { }
                            },
                            Identifier { Token { "v", "test.basalt", 1, 1, 1, Token::Type::text } },
                            Token { const_keyword, "test.basalt", 1, 1, 1, Token::Type::const_keyword }
                        },
                        Assignment {
                            Identifier { Token { "v", "test.basalt", 1, 1, 1, Token::Type::text } },
                            Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
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

TEST(Preprocessing, Immutability_Constraint_Validator_Assignment_Of_Const_Ptr_To_Var_Ptr) {
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
                            "v",
                            TypeSignatureFactory::PointerToInt,
                            std::nullopt,
                            Token { var_keyword, "test.basalt", 1, 1, 1, Token::Type::var_keyword }
                        },
                        ConstDeclaration {
                            "x",
                            TypeSignatureFactory::PointerToInt,
                            Identifier { Token { "v", "test.basalt", 1, 1, 1, Token::Type::text } },
                            Token { const_keyword, "test.basalt", 1, 1, 1, Token::Type::const_keyword }
                        },
                        Assignment {
                            Identifier { Token { "v", "test.basalt", 1, 1, 1, Token::Type::text } },
                            Identifier { Token { "x", "test.basalt", 1, 1, 1, Token::Type::text } },
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