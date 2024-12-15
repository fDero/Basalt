
#include <gtest/gtest.h>
#include "preprocessing/address_sanitizer.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"

TEST(Preprocessing, Const_Constraint_Validator_Assignment_Of_Ref_To_Array_Literal_To_Const_Slice) {
    ProjectFileStructure single_func_def_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
            },
            .func_defs = { 
                FunctionDefinitionFactory::make_function_definition(
                    "f", "test.basalt", {}, {}, 
                    std::nullopt,
                    {
                        ConstDeclaration {
                            "x",
                            TypeSignatureFactory::SliceOfInts,
                            UnaryOperator {
                                Token { "&", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                ArrayLiteral {
                                    std::nullopt, 
                                    TypeSignatureFactory::Int, {
                                        IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                        IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                        IntLiteral { Token { "2", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
                                    },
                                    Token { "[", "test.basalt", 1, 1, 4, Token::Type::type }
                                },
                            },
                            Token { "const", "test.basalt", 1, 1, 1, Token::Type::const_keyword }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    AddressSanitizer address_sanitzier(single_func_def_program);
    EXPECT_ANY_THROW(address_sanitzier.visit_all_function_definitions());
}

TEST(Preprocessing, Const_Constraint_Validator_Assignment_Of_Ref_To_Array_Literal_To_Var_Slice) {
    ProjectFileStructure single_func_def_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
            },
            .func_defs = { 
                FunctionDefinitionFactory::make_function_definition(
                    "f", "test.basalt", {}, {}, 
                    std::nullopt,
                    {
                        ConstDeclaration {
                            "x",
                            TypeSignatureFactory::SliceOfInts,
                            UnaryOperator {
                                Token { "&", "test.basalt", 1, 1, 1, Token::Type::symbol },
                                ArrayLiteral {
                                    std::nullopt, 
                                    TypeSignatureFactory::Int, {
                                        IntLiteral { Token { "0", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                        IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                                        IntLiteral { Token { "2", "test.basalt", 1, 1, 1, Token::Type::integer_literal } }
                                    },
                                    Token { "[", "test.basalt", 1, 1, 4, Token::Type::type }
                                },
                            },
                            Token { "const", "test.basalt", 1, 1, 1, Token::Type::const_keyword }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    AddressSanitizer address_sanitzier(single_func_def_program);
    EXPECT_ANY_THROW(address_sanitzier.visit_all_function_definitions());
}