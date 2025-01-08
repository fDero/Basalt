//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/function_call_resolver.hpp"

FunctionCallResolver::FunctionCallResolver(
    TypeDefinitionsRegister& type_definitions_register,
    OverloadingResolutionEngine& overloading_resolution_engine,
    CommonFeatureAdoptionPlanGenerationEngine& common_feature_adoption_plan_generation_engine
)
    : type_definitions_register(type_definitions_register)
    , overloading_resolution_engine(overloading_resolution_engine)
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
    CallableCodeBlock callable_code_block = resolve_function_call(function_call, argument_types);
    return as_return_type(callable_code_block.get_return_type());
}

FunctionCallResolver::ReturnType
FunctionCallResolver::as_return_type(std::optional<TypeSignature> type) {
    return (type.has_value())
        ? (ReturnType) type.value()
        : (ReturnType) Void();
}

CallableCodeBlock FunctionCallResolver::resolve_function_call(
    const FunctionCall& function_call,
    const std::vector <TypeSignature>& argument_types
) {
    FunctionDefinition::Ref retrieved = overloading_resolution_engine
        .retrieve_function_definition(function_call, argument_types);
    if (retrieved != nullptr) {
        return {retrieved, type_definitions_register};
    }
    auto cfa = common_feature_adoption_plan_generation_engine
        .generate_common_feature_adoption_plan_descriptor(function_call, argument_types);
    return {cfa, type_definitions_register};
}