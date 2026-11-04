//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <optional>
#include <vector>

#include "language/typesignatures.hpp"
#include "misc/forward_declarations.hpp"
#include "misc/smart_variant.hpp"


class FunctionCallResolver {
    public:
        FunctionCallResolver(
            TypeDefinitionsRegister& type_definitions_register,
            FunctionDefinitionsRegister& function_definitions_register,
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
        FunctionDefinitionsRegister& function_definitions_register;
        CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine;
};
