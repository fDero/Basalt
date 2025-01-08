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
#include "core/callable_code_block.hpp"
#include "core/caching_aware_register.hpp"

class FunctionCallResolver {

    public:
        FunctionCallResolver(
            TypeDefinitionsRegister& type_definitions_register,
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

        [[nodiscard]] CallableCodeBlock resolve_function_call(
            const FunctionCall& function_call,
            const std::vector<TypeSignature>& argument_types
        );

    protected:
        [[nodiscard]] ReturnType as_return_type(std::optional<TypeSignature> type);

    private:
        TypeDefinitionsRegister& type_definitions_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
        CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine;
};