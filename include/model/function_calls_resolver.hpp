//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

#include "model/function_overloads_register.hpp"
#include "model/overloading_resolution_engine.hpp"
#include "model/common_feature_adoption_plan_generation_engine.hpp"

class FunctionCallsResolver {

    public:
        FunctionCallsResolver(
            FunctionOverloadsRegister& functions_overloads_register,
            OverloadingResolutionEngine& overloading_resolution_engine,
            CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine
        );

        std::optional<TypeSignature> resolve_function_call_return_type(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

    private:
        FunctionOverloadsRegister& functions_overloads_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
        CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine;
};