
#include <gtest/gtest.h>
#include "core/function_overloads_register.hpp"
#include "core/type_definitions_register.hpp"
#include "core/common_feature_adoption_plan_generation_engine.hpp"
#include "core/common_feature_adoption_plan_descriptor.hpp"
#include "frontend/syntax.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

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
    FunctionOverloadsRegister function_register(project_with_single_file_containing_two_overload_of_the_add_function);
    TypeDefinitionsRegister type_register(project_with_single_file_containing_two_overload_of_the_add_function);
    OverloadingResolutionEngine overloading_resolution_engine(function_register, type_register, project_with_single_file_containing_two_overload_of_the_add_function);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(overloading_resolution_engine, type_register);
    EXPECT_ANY_THROW(
        common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
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
    FunctionOverloadsRegister function_register(project_with_single_file_containing_two_overload_of_the_f_function_with_incompatible_return_types);
    TypeDefinitionsRegister type_register(project_with_single_file_containing_two_overload_of_the_f_function_with_incompatible_return_types);
    OverloadingResolutionEngine overloading_resolution_engine(function_register, type_register, project_with_single_file_containing_two_overload_of_the_f_function_with_incompatible_return_types);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(overloading_resolution_engine, type_register);
    EXPECT_ANY_THROW(
        common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
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