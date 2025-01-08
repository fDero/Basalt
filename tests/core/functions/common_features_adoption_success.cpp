
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

ProjectFileStructure project_with_single_file_containing_four_overloads_of_the_add_function ({
    FileRepresentation {
        .file_metadata = { 
            .filename = "a.basalt",
            .packagename = "apackage",
            .imports = { "bpackage", "cpackage" },
        },
        .type_defs = {},
        .func_defs = {
            FunctionDefinitionFactory::make_function_definition(
                "add",
                "a.basalt", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::Int },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::Int }
                },
                TypeSignatureFactory::Int
            ),
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
            ),
            FunctionDefinitionFactory::make_function_definition(
                "add",
                "a.basalt", 
                FunctionDefinitionFactory::no_generics, 
                {
                    FunctionDefinition::Argument { "a", TypeSignatureFactory::Int },
                    FunctionDefinition::Argument { "b", TypeSignatureFactory::Float }
                },
                TypeSignatureFactory::Float
            )
        }
    }
});


TEST(Core, CFA_Success_When_It_Is_Not_Necessary) {
    FunctionOverloadsRegister function_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    TypeDefinitionsRegister type_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    OverloadingResolutionEngine overloading_resolution_engine(function_register, type_register, project_with_single_file_containing_four_overloads_of_the_add_function);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(overloading_resolution_engine, type_register);
    auto cfa_plan_description = common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
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
            TypeSignatureFactory::Int,
            TypeSignatureFactory::Int
        }
    );
    CommonFeatureAdoptionPlan cfa_plan = cfa_plan_description.plan;
    ASSERT_TRUE(cfa_plan.is_direct_adoption());
}


TEST(Core, CFA_Success_When_It_Is_Not_Necessary2) {
    FunctionOverloadsRegister function_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    TypeDefinitionsRegister type_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    OverloadingResolutionEngine overloading_resolution_engine(function_register, type_register, project_with_single_file_containing_four_overloads_of_the_add_function);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(overloading_resolution_engine, type_register);
    auto cfa_plan_descriptor = common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
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
            TypeSignatureFactory::Float,
            TypeSignatureFactory::Int
        }
    );
    CommonFeatureAdoptionPlan& cfa_plan = cfa_plan_descriptor.plan;
    EXPECT_TRUE(cfa_plan.is_direct_adoption());
}


TEST(Core, CFA_Success_Resolving_One_Union_As_First_Arg) {
    FunctionOverloadsRegister function_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    TypeDefinitionsRegister type_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    OverloadingResolutionEngine overloading_resolution_engine(function_register, type_register, project_with_single_file_containing_four_overloads_of_the_add_function);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(overloading_resolution_engine, type_register);
    auto cfa_plan_descriptor = common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
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
            TypeSignatureFactory::Int
        }
    );
    CommonFeatureAdoptionPlan cfa_plan = cfa_plan_descriptor.plan;
    ASSERT_TRUE(cfa_plan.is_recursive_adoption());
    EXPECT_EQ(cfa_plan.get_recursive_adoption().alternatives.size(), 2);
    EXPECT_EQ(cfa_plan.get_recursive_adoption().nested_plans.size(), 2);
    EXPECT_EQ(cfa_plan.get_recursive_adoption().argument_index, 0);
    CommonFeatureAdoptionPlan nested_plan_1 = cfa_plan.get_recursive_adoption().nested_plans[0];
    CommonFeatureAdoptionPlan nested_plan_2 = cfa_plan.get_recursive_adoption().nested_plans[1];
    EXPECT_TRUE(nested_plan_1.is_direct_adoption());
    EXPECT_TRUE(nested_plan_2.is_direct_adoption());
    ASSERT_TRUE(cfa_plan_descriptor.return_type.has_value());
    EXPECT_TRUE(cfa_plan_descriptor.return_type.value().is<InlineUnion>());
}


TEST(Core, CFA_Success_Resolving_Two_Unions) {
    FunctionOverloadsRegister function_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    TypeDefinitionsRegister type_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    OverloadingResolutionEngine overloading_resolution_engine(function_register, type_register, project_with_single_file_containing_four_overloads_of_the_add_function);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(overloading_resolution_engine, type_register);
    auto cfa_plan_descriptor = common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
        FunctionCall {
            Token { "add", "a.basalt", 1, 1, 1, Token::Type::text },
            { 
                Identifier { Token { "x", "a.basalt", 1, 1, 1, Token::Type::text } },
                Identifier { Token { "y", "a.basalt", 1, 1, 1, Token::Type::text } } 
            }, 
            {
                //no explicit type parameters section
            }
        },
        {
            TypeSignatureFactory::IntOrFloat,
            TypeSignatureFactory::IntOrFloat
        }
    );
    CommonFeatureAdoptionPlan cfa_plan = cfa_plan_descriptor.plan;
    ASSERT_TRUE(cfa_plan.is_recursive_adoption());
    EXPECT_EQ(cfa_plan.get_recursive_adoption().alternatives.size(), 2);
    EXPECT_EQ(cfa_plan.get_recursive_adoption().nested_plans.size(), 2);
    EXPECT_EQ(cfa_plan.get_recursive_adoption().argument_index, 0);
    CommonFeatureAdoptionPlan nested_plan_1 = cfa_plan.get_recursive_adoption().nested_plans[0];
    CommonFeatureAdoptionPlan nested_plan_2 = cfa_plan.get_recursive_adoption().nested_plans[1];
    EXPECT_EQ(nested_plan_1.get_recursive_adoption().alternatives.size(), 2);
    EXPECT_EQ(nested_plan_1.get_recursive_adoption().nested_plans.size(), 2);
    EXPECT_EQ(nested_plan_1.get_recursive_adoption().argument_index, 1);
    EXPECT_EQ(nested_plan_2.get_recursive_adoption().alternatives.size(), 2);
    EXPECT_EQ(nested_plan_2.get_recursive_adoption().nested_plans.size(), 2);
    EXPECT_EQ(nested_plan_2.get_recursive_adoption().argument_index, 1);
    ASSERT_TRUE(cfa_plan_descriptor.return_type.has_value());
    EXPECT_TRUE(cfa_plan_descriptor.return_type.value().is<InlineUnion>());
}

TEST(Core, CFA_Success_Resolving_One_Union_As_Second_Arg) {
    FunctionOverloadsRegister function_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    TypeDefinitionsRegister type_register(project_with_single_file_containing_four_overloads_of_the_add_function);
    OverloadingResolutionEngine overloading_resolution_engine(function_register, type_register, project_with_single_file_containing_four_overloads_of_the_add_function);
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generator(overloading_resolution_engine, type_register);
    auto cfa_plan_descriptor = common_feature_adoption_plan_generator.generate_common_feature_adoption_plan_descriptor(
        FunctionCall {
            Token { "add", "a.basalt", 1, 1, 1, Token::Type::text },
            { 
                IntLiteral { Token { "5", "a.basalt", 1, 1, 1, Token::Type::integer_literal } },
                Identifier { Token { "y", "a.basalt", 1, 1, 1, Token::Type::text } } 
            }, 
            {
                //no explicit type parameters section
            }
        },
        {
            TypeSignatureFactory::Int,
            TypeSignatureFactory::IntOrFloat
        }
    );
    CommonFeatureAdoptionPlan cfa_plan = cfa_plan_descriptor.plan;
    ASSERT_TRUE(cfa_plan.is_recursive_adoption());
    EXPECT_EQ(cfa_plan.get_recursive_adoption().alternatives.size(), 2);
    EXPECT_EQ(cfa_plan.get_recursive_adoption().nested_plans.size(), 2);
    EXPECT_EQ(cfa_plan.get_recursive_adoption().argument_index, 1);
    CommonFeatureAdoptionPlan nested_plan_1 = cfa_plan.get_recursive_adoption().nested_plans[0];
    CommonFeatureAdoptionPlan nested_plan_2 = cfa_plan.get_recursive_adoption().nested_plans[1];
    EXPECT_TRUE(nested_plan_1.is_direct_adoption());
    EXPECT_TRUE(nested_plan_2.is_direct_adoption());
    ASSERT_TRUE(cfa_plan_descriptor.return_type.has_value());
    EXPECT_TRUE(cfa_plan_descriptor.return_type.value().is<InlineUnion>());
}