
#pragma once

#include "model/function_overloads_register.hpp"
#include "model/overloading_resolution_engine.hpp"
#include "model/type_definitions_register.hpp"
#include "model/project_file_structure.hpp"
#include "model/common_feature_adoption_plan_descriptor.hpp"

class CommonFeatureAdoptionPlanGenerationEngine {
    
    public:
        CommonFeatureAdoptionPlanGenerationEngine(
            OverloadingResolutionEngine& overloading_resolution_engine,
            TypeDefinitionsRegister& type_definitions_register
        );

        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_plan(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

    private:
        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_iterating_over_arg_types(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types,
            std::vector<TypeSignature>::const_iterator current_arg_type
        );

        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_for_inline_union(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types,
            std::vector<TypeSignature>::const_iterator current_arg_type
        );

        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_for_custom_type(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types,
            std::vector<TypeSignature>::const_iterator current_arg_type
        );

        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_for_current_multicase_arg(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types,
            std::vector<TypeSignature>::const_iterator current_arg_type,
            const std::vector<TypeSignature>& alternatives
        );

        OverloadingResolutionEngine& overloading_resolution_engine;
        TypeDefinitionsRegister& type_definitions_register;
};
