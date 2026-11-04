#include <tuple>

#include <gtest/gtest.h>

#include "core/common_feature_adoption_plan_generation_engine.hpp"
#include "core/function_definitions_register.hpp"
#include "core/project_file_structure.hpp"
#include "core/type_definitions_register.hpp"
#include "frontend/file_representation.hpp"
#include "frontend/token.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"
#include "../../tests_utilities/compilation_error_assertions.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"


ProjectFileStructure project_with_single_file_containing_two_overload_of_the_add_function ({
    FileRepresentation {
        .file_metadata = { 
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { },
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add",
                "a.basalt", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::Float },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::Float }
                },
                TypeSignatureFactory::Float
            ),
            FunctionDefinitionFactory::make_function_definition(
                "add",
                "a.basalt", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::Float },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::Int }
                },
                TypeSignatureFactory::Float
            )
        }
    }
});


TEST(Core, CFA_Failure_Not_Every_Case_Is_Covered) {
    TypeDefinitionsRegister type_register(project_with_single_file_containing_two_overload_of_the_add_function);
    FunctionDefinitionsRegister function_definitions_register(type_register, project_with_single_file_containing_two_overload_of_the_add_function);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(function_definitions_register, type_register);
    EXPECT_COMPILATION_ERROR(IndexingError, "no overload of", 
        std::ignore = common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
            FunctionCall {
                Token { "add", "a.basalt", 1, 1, 1, Token::Type::text },
                { 
                    Identifier { Token { "x", "a.basalt", 1, 1, 1, Token::Type::text } },
                    IntLiteral { Token { "7", "a.basalt", 1, 1, 1, Token::Type::integer_literal } } 
                }, 
                {
                    //no explicit type parameters section
                }
            },
            {
                TypeSignatureFactory::IntOrFloat,
                TypeSignatureFactory::IntOrFloat
            }
        )
    );
}


ProjectFileStructure project_with_single_file_containing_two_overload_of_the_f_function_with_incompatible_return_types ({
    FileRepresentation {
        .file_metadata = { 
            .filename = "f.basalt",
            .packagename = "fpackage",
            .imports = { },
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_void_function_definition(
                "f",
                "f.basalt", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "x", TypeSignatureFactory::Float },
                    FunctionDefinition::Argument { "y", TypeSignatureFactory::Float }
                }
            ),
            FunctionDefinitionFactory::make_function_definition(
                "f",
                "f.basalt", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::Float },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::Int }
                },
                TypeSignatureFactory::Float
            )
        }
    }
});


TEST(Core, CFA_Failure_Incompatible_Return_Types) {
    TypeDefinitionsRegister type_register(project_with_single_file_containing_two_overload_of_the_f_function_with_incompatible_return_types);
    FunctionDefinitionsRegister function_definitions_register(type_register, project_with_single_file_containing_two_overload_of_the_f_function_with_incompatible_return_types);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(function_definitions_register, type_register);
    EXPECT_COMPILATION_ERROR(ValidationError, "some of the overloads it dispatches to return a value and others don't", 
        std::ignore = common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
            FunctionCall {
                Token { "f", "f.basalt", 1, 1, 1, Token::Type::text },
                { 
                    Identifier { Token { "x", "f.basalt", 1, 1, 1, Token::Type::text } },
                    IntLiteral { Token { "7", "f.basalt", 1, 1, 1, Token::Type::integer_literal } } 
                }, 
                {
                    //no explicit type parameters section
                }
            },
            {
                TypeSignatureFactory::Float,
                TypeSignatureFactory::IntOrFloat
            }
        )
    );
}