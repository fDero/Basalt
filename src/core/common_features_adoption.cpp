//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/common_feature_adoption_plan_descriptor.hpp"
#include "core/common_feature_adoption_plan_generation_engine.hpp"
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"

CommonFeatureAdoptionPlanGenerationEngine::CommonFeatureAdoptionPlanGenerationEngine( 
    OverloadingResolutionEngine& overloading_resolution_engine,
    TypeDefinitionsRegister& type_definitions_register
) 
    : overloading_resolution_engine(overloading_resolution_engine)
    , type_definitions_register(type_definitions_register)
{ }

CommonFeatureAdoptionPlanDescriptor
CommonFeatureAdoptionPlanGenerationEngine::generate_common_feature_adoption_plan_descriptor(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& arg_types
) {
    CommonFeatureAdoptionPlan plan = generate_common_feature_adoption_plan(function_call, arg_types);
    std::vector<std::optional<TypeSignature>> return_types = isolate_return_types(plan);
    std::optional<TypeSignature> return_type = compute_recursive_adoption_plan_return_type(function_call, return_types);
    return CommonFeatureAdoptionPlanDescriptor {
        function_call.function_name,
        arg_types,
        return_type,
        function_call.as_debug_informations_aware_entity().filename,
        function_call.as_debug_informations_aware_entity(),
        plan
    };
}

CommonFeatureAdoptionPlan 
CommonFeatureAdoptionPlanGenerationEngine::generate_common_feature_adoption_plan(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& arg_types
) { 
    std::string cache_key = get_cache_search_key_for_func_def_retrieval_from_func_call(
        type_definitions_register, function_call, arg_types
    );
    auto cache_search_outcome = fast_retrieve_cache.find(cache_key);
    if (cache_search_outcome != fast_retrieve_cache.end()) {
        return cache_search_outcome->second;
    }
    auto cfa_plan = generate_common_feature_adoption_iterating_over_arg_types(
        function_call, 
        arg_types, 
        arg_types.begin()
    );
    fast_retrieve_cache.insert({cache_key, cfa_plan});
    return cfa_plan;
}

CommonFeatureAdoptionPlan
CommonFeatureAdoptionPlanGenerationEngine::generate_common_feature_adoption_iterating_over_arg_types(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& arg_types,
    std::vector<TypeSignature>::const_iterator current_arg_type_iterator
) { 
    FunctionDefinition::Ref retrieved = overloading_resolution_engine.retrieve_function_definition(function_call, arg_types);
    CommonFeatureAdoptionPlan direct_adoption_plan = retrieved;
    if (retrieved != nullptr) {
        return direct_adoption_plan;
    }
    ensure_common_feature_adoption_is_possible(current_arg_type_iterator, arg_types.end());
    TypeSignature current_arg_type = type_definitions_register.unalias_type(*current_arg_type_iterator);
    assert_current_arg_type_is_not_generic(current_arg_type);
    switch (current_arg_type.typesiganture_kind()) {
        case TypeSignatureBody::Kind::inline_union:return generate_common_feature_adoption_for_inline_union(
            function_call, arg_types, current_arg_type_iterator
        ); 
        case TypeSignatureBody::Kind::custom_type: return generate_common_feature_adoption_for_custom_type(
            function_call, arg_types, current_arg_type_iterator
        );
        default: return generate_common_feature_adoption_iterating_over_arg_types(
            function_call, arg_types, std::next(current_arg_type_iterator)
        );
    }
}

CommonFeatureAdoptionPlan
CommonFeatureAdoptionPlanGenerationEngine::generate_common_feature_adoption_for_inline_union(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& arg_types,
    std::vector<TypeSignature>::const_iterator current_arg_type_iterator
) {
    assert_current_arg_type_is_not_generic(*current_arg_type_iterator);
    assert_typesignature_is<InlineUnion>(*current_arg_type_iterator);
    InlineUnion inline_union = current_arg_type_iterator->get<InlineUnion>();
    std::vector<TypeSignature> union_alternatives = inline_union.alternatives;
    return generate_common_feature_adoption_for_current_multicase_arg(
        function_call, arg_types, current_arg_type_iterator, union_alternatives
    );
}

CommonFeatureAdoptionPlan
CommonFeatureAdoptionPlanGenerationEngine::generate_common_feature_adoption_for_custom_type(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& arg_types,
    std::vector<TypeSignature>::const_iterator current_arg_type_iterator
) {
    assert_current_arg_type_is_not_generic(*current_arg_type_iterator);
    assert_typesignature_is<CustomType>(*current_arg_type_iterator);
    CustomType custom_type = current_arg_type_iterator->get<CustomType>();
    TypeSignature unaliased_type = type_definitions_register.unalias_type(custom_type);
    if (unaliased_type.typesiganture_kind() != TypeSignatureBody::Kind::custom_type) {
        return generate_common_feature_adoption_iterating_over_arg_types(
            function_call, arg_types, std::next(current_arg_type_iterator)
        );
    }
    CustomType unaliased_custom_type = unaliased_type.get<CustomType>();
    TypeDefinition type_definition = type_definitions_register.retrieve_type_definition(unaliased_custom_type);
    if (!type_definition.is<UnionDefinition>()) {
        return generate_common_feature_adoption_iterating_over_arg_types(
            function_call, arg_types, std::next(current_arg_type_iterator)
        );
    }
    std::vector<TypeSignature> union_alternatives = type_definition.get<UnionDefinition>().types;
    return generate_common_feature_adoption_for_current_multicase_arg(
        function_call, arg_types, current_arg_type_iterator, union_alternatives
    );
}

CommonFeatureAdoptionPlan
CommonFeatureAdoptionPlanGenerationEngine::generate_common_feature_adoption_for_current_multicase_arg(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& arg_types,
    std::vector<TypeSignature>::const_iterator current_arg_type_iterator,
    const std::vector<TypeSignature>& alternatives
) {
    assert_current_arg_type_is_not_generic(*current_arg_type_iterator);
    RecursiveAdoptionPlan plan;
    plan.argument_index = static_cast<size_t>(std::distance(arg_types.begin(), current_arg_type_iterator));
    for (const TypeSignature& alternative : alternatives) { 
        std::vector<TypeSignature> new_arg_types;
        std::copy(arg_types.begin(), current_arg_type_iterator, std::back_inserter(new_arg_types));
        new_arg_types.push_back(alternative);
        std::copy(std::next(current_arg_type_iterator), arg_types.end(), std::back_inserter(new_arg_types));
        std::vector<TypeSignature>::const_iterator new_arg_type_iterator = new_arg_types.begin();
        size_t new_arg_type_iterator_offset = std::distance(arg_types.begin(), current_arg_type_iterator);
        new_arg_type_iterator += new_arg_type_iterator_offset;
        CommonFeatureAdoptionPlan nested_plan = generate_common_feature_adoption_iterating_over_arg_types(
            function_call, new_arg_types, new_arg_type_iterator
        );
        
        plan.alternatives.push_back(alternative);
        plan.nested_plans.emplace_back(std::move(nested_plan));
    }
    return plan;
}

std::vector<std::optional<TypeSignature>> 
CommonFeatureAdoptionPlanGenerationEngine::isolate_return_types(
    const CommonFeatureAdoptionPlan& plan
){
    if (plan.is_direct_adoption()) {
        FunctionDefinition::Ref direct_adoption = plan.get_direct_adoption();
        return { direct_adoption->return_type };
    }
    assert_is_recursive_cfa_plan(plan);
    std::vector<std::optional<TypeSignature>> return_types;
    for (const CommonFeatureAdoptionPlan& nested_plan : plan.get_recursive_adoption().nested_plans) {
        for (const std::optional<TypeSignature>& return_type : isolate_return_types(nested_plan)) {
            return_types.push_back(return_type);
        }
    }
    return return_types;
}

std::optional<TypeSignature> 
CommonFeatureAdoptionPlanGenerationEngine::compute_recursive_adoption_plan_return_type(
    const FunctionCall& function_call,
    std::vector<std::optional<TypeSignature>>& return_types
) {
    bool contains_void_return_type = false;
    bool contains_non_void_return_type = false;
    std::vector<TypeSignature> unwrapped_return_types;
    for (const std::optional<TypeSignature>& return_type : return_types) {
        contains_void_return_type |= !return_type.has_value();
        contains_non_void_return_type |= return_type.has_value();
        if (return_type.has_value()) {
            unwrapped_return_types.push_back(return_type.value());
        }
    }
    ensure_common_feature_adoption_is_possible(function_call, contains_void_return_type, contains_non_void_return_type);
    return (contains_void_return_type)
        ? std::optional<TypeSignature>(std::nullopt) 
        : InlineUnion ( 
            function_call.as_debug_informations_aware_entity(), 
            unwrapped_return_types 
        );
}