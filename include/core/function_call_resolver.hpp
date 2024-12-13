//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/project_file_structure.hpp"
#include "core/type_definitions_register.hpp"
#include "core/function_overloads_register.hpp"
#include "core/overloading_resolution_engine.hpp"
#include "core/common_feature_adoption_plan_generation_engine.hpp"
#include "core/scope_context.hpp"

class FunctionCallResolver {

    public:
        FunctionCallResolver(
            OverloadingResolutionEngine& overloading_resolution_engine,
            CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine
        );

        struct Void { };
        struct Unresolved { };
        using ReturnType = SmartVariant<TypeSignature, Void, Unresolved>;

        [[nodiscard]] ReturnType resolve_function_call_return_type(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& argument_types
        );

    protected:
        [[nodiscard]] ReturnType as_return_type(std::optional<TypeSignature> type);

    private:
        OverloadingResolutionEngine& overloading_resolution_engine;
        CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine;
};