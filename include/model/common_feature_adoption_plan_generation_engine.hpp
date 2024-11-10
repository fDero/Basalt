//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <unordered_map>

#include "model/function_overloads_register.hpp"
#include "model/overloading_resolution_engine.hpp"
#include "model/type_definitions_register.hpp"
#include "model/project_file_structure.hpp"
#include "model/common_feature_adoption_plan_descriptor.hpp"
#include "model/caching_aware_register.hpp"

class CommonFeatureAdoptionPlanGenerationEngine : public CachingAwareRegister {
    
    public:
        CommonFeatureAdoptionPlanGenerationEngine(
            OverloadingResolutionEngine& overloading_resolution_engine,
            TypeDefinitionsRegister& type_definitions_register
        );

        CommonFeatureAdoptionPlanDescriptor generate_common_feature_adoption_plan(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

    protected:
        using CachingAwareRegister::get_cache_search_key_for_func_def_retrieval_from_func_call;

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

        std::unordered_map<std::string, CommonFeatureAdoptionPlanDescriptor> fast_retrieve_cache;
        OverloadingResolutionEngine& overloading_resolution_engine;
        TypeDefinitionsRegister& type_definitions_register;
};