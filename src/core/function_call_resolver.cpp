//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/function_call_resolver.hpp"

FunctionCallResolver::FunctionCallResolver(
    OverloadingResolutionEngine& overloading_resolution_engine,
    CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine
)
    : overloading_resolution_engine(overloading_resolution_engine)
    , common_feature_adoption_plan_generation_engine(common_feature_adoption_plan_generation_engine)
{}

FunctionCallResolver::ReturnType
FunctionCallResolver::resolve_function_call_return_type(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& argument_types
) {
   for (const TypeSignature& argument_type : argument_types) {
        if (argument_type.is_generic()) {
            return Unresolved();
        }
    }
    FunctionDefinition::Ref retrieved = 
        overloading_resolution_engine
            .retrieve_function_definition(function_call, argument_types);
    if (retrieved != nullptr) {
        std::optional<TypeSignature> func_return_type = retrieved->return_type;
        return as_return_type(func_return_type);
    }
    CommonFeatureAdoptionPlanDescriptor cfa = 
        common_feature_adoption_plan_generation_engine
            .generate_common_feature_adoption_plan(function_call, argument_types);
    return as_return_type(cfa.get_return_type());
}

FunctionCallResolver::ReturnType
FunctionCallResolver::as_return_type(std::optional<TypeSignature> type) {
    return (type.has_value())
        ? (ReturnType) type.value()
        : (ReturnType) Void();
}