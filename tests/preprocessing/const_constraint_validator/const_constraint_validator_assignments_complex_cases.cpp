
#include <gtest/gtest.h>
#include "preprocessing/const_constraint_validator.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"

TEST(Preprocessing, Const_Constraint_Validator_Assignment_Of_Literal_To_Member_Of_A_Struct_Variable) {
    ProjectFileStructure single_func_def_project({
        FileRepresentation {
            .file_metadata = {
                .filename = "test.basalt",
                .packagename = "mainpackage",
                .imports = { }
            },
            .type_defs = { 
                StructDefinitionFactory::make_struct_definition(
                    "IntWrapper", "test.basalt", {}, { 
                        StructDefinition::Field { 
                            "wrapped", TypeSignatureFactory::Int 
                        }
                    }
                )
            },
            .func_defs = { 
                FunctionDefinitionFactory::make_function_definition(
                    "f", "test.basalt", {}, {}, 
                    CustomType {
                        Token { "IntWrapper", "test.basalt", 1, 1, 1, Token::Type::type }, {}
                    },
                    {
                        VariableDeclaration {
                            "v",
                            CustomType {
                                Token { "IntWrapper", "test.basalt", 1, 1, 1, Token::Type::type },
                                {}
                            },
                            std::nullopt,
                            Token { "var", "test.basalt", 1, 1, 1, Token::Type::var_keyword }
                        },
                        Assignment {
                            Identifier { Token { "v", "test.basalt", 1, 1, 1, Token::Type::text } },
                            IntLiteral { Token { "1", "test.basalt", 1, 1, 1, Token::Type::integer_literal } },
                            Token { "=", "test.basalt", 1, 1, 1, Token::Type::symbol }
                        }
                    }
                )
            }
        }
    });
    ProgramRepresentation single_func_def_program(single_func_def_project);
    ConstConstraintValidator const_constraint_validator(single_func_def_program);
    const_constraint_validator.visit_all_function_definitions();
}