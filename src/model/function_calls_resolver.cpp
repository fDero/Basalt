//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "model/function_calls_resolver.hpp"

FunctionCallsResolver::FunctionCallsResolver(
    FunctionOverloadsRegister& functions_overloads_register,
    OverloadingResolutionEngine& overloading_resolution_engine,
    CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine
)
    : functions_overloads_register(functions_overloads_register)
    , overloading_resolution_engine(overloading_resolution_engine)
    , common_feature_adoption_plan_generation_engine(common_feature_adoption_plan_generation_engine)
{ }

std::optional<TypeSignature> 
FunctionCallsResolver::resolve_function_call_return_type(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& arg_types
) {
    FunctionDefinition::Ref retrieved = 
        overloading_resolution_engine
            .retrieve_function_definition(function_call, arg_types);
    if (retrieved != nullptr) {
        return retrieved->return_type;
    }
    CommonFeatureAdoptionPlanDescriptor cfa = 
        common_feature_adoption_plan_generation_engine
            .generate_common_feature_adoption_plan(function_call, arg_types);
    return cfa.get_return_type();
}