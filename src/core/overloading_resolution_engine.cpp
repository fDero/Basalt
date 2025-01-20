//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/function_specificity_descriptor.hpp"
#include "core/overloading_resolution_engine.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"
#include "core/generics_instantiation_engine.hpp"
#include "preprocessing/preprocessor.hpp"
#include "core/assignment_type_checker.hpp"

OverloadingResolutionEngine::OverloadingResolutionEngine(
    FunctionOverloadsRegister& function_overloads_register,
    TypeDefinitionsRegister& type_definitions_register,
    ProjectFileStructure& project_file_structure
)
    : function_overloads_register(function_overloads_register)
    , type_definitions_register(type_definitions_register) 
    , project_file_structure(project_file_structure)
{}

FunctionDefinition::Ref OverloadingResolutionEngine::retrieve_function_definition(
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
) {
    std::string cache_key = get_cache_search_key_for_func_def_retrieval_from_func_call(type_definitions_register, function_call, arg_types);
    auto cache_search_outcome = fast_retrieve_cache.find(cache_key);
    if (cache_search_outcome != fast_retrieve_cache.end()) {
        return cache_search_outcome->second;
    }
    FunctionDefinition::Ref func_def_ref = cache_unaware_function_definition_retrieval(function_call, arg_types);
    fast_retrieve_cache.insert({cache_key, func_def_ref});
    return func_def_ref;
}

std::vector<OverloadingResolutionEngine::MatchedFunctionData>
OverloadingResolutionEngine::search_for_best_matches(
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
) {
    std::vector<std::string> candidate_overload_set_ids = function_overloads_register.retrieve_overload_sets_ids(function_call);
    std::vector<std::pair<FunctionDefinition::Ref, GenericSubstitutionRule::Set::Ref>> best_maches_so_far; 
    FunctionSpecificityDescriptor best_specificity_so_far = FunctionSpecificityDescriptor::worst_possible_specificity();
    for (const std::string& overload_set_id : candidate_overload_set_ids) {
        std::vector<FunctionDefinition::Ref>& current_overload_set = function_overloads_register.retrieve_specific_overload_set(overload_set_id);
        for (const FunctionDefinition::Ref& func_def_ref : current_overload_set) {
            FunctionSpecificityDescriptor current_specificity(*func_def_ref, type_definitions_register);
            FunctionSpecificityDescriptor::ComparisonResult comparison_result = current_specificity.compare_with(best_specificity_so_far);
            bool is_less_specific = comparison_result == FunctionSpecificityDescriptor::ComparisonResult::less_specific;
            GenericSubstitutionRule::Set::Ref generic_substitution_rules_ref = check_function_compatibility(func_def_ref, function_call, arg_types);
            bool should_be_ignored = is_less_specific || generic_substitution_rules_ref == nullptr;
            if (should_be_ignored) {
                continue;
            }
            best_maches_so_far.push_back({ func_def_ref, generic_substitution_rules_ref });
            if (comparison_result == FunctionSpecificityDescriptor::ComparisonResult::more_specific) {
                best_maches_so_far = { { func_def_ref, generic_substitution_rules_ref } };
                best_specificity_so_far = current_specificity;
            }
        }
    } 
    return best_maches_so_far;
}

std::string OverloadingResolutionEngine::get_new_instantiated_function_name(
    const FunctionDefinition& function_definition,
    GenericSubstitutionRule::Set::Ref generic_substitution_rules
) {
    if (generic_substitution_rules == nullptr || generic_substitution_rules->empty()) {
        return function_definition.function_name;
    }
    std::string new_function_name = function_definition.function_name + "<";
    for (const GenericSubstitutionRule& rule : *generic_substitution_rules) {
        new_function_name += type_definitions_register.get_fully_qualified_typesignature_name(rule.replacement) + ",";
    }
    new_function_name.back() = '>';
    return new_function_name;
}

FunctionDefinition::Ref OverloadingResolutionEngine::cache_unaware_function_definition_retrieval(
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
) {
    std::vector<MatchedFunctionData> best_maches_so_far = search_for_best_matches(function_call, arg_types);
    ensure_no_multiple_ambiguous_candidate_function_overloads_have_been_found(best_maches_so_far);
    if (best_maches_so_far.empty()) {
        return nullptr;
    }
    const FunctionDefinition& best_match = *best_maches_so_far[0].first; 
    const GenericSubstitutionRule::Set::Ref generic_substitution_rules = best_maches_so_far[0].second;
    const std::string new_function_name = get_new_instantiated_function_name(best_match, generic_substitution_rules);
    GenericsInstantiationEngine generics_instantiation_engine(*generic_substitution_rules);
    FunctionDefinition::Ref instanitated_func_def_ref = 
        generics_instantiation_engine.instantiate_generic_function(best_match, new_function_name);
    return instanitated_func_def_ref;
}

GenericSubstitutionRule::Set::Ref OverloadingResolutionEngine::check_function_compatibility(
    const FunctionDefinition::Ref func_def_ref,
    const FunctionCall& func_call,
    const std::vector<TypeSignature>& arg_types
) {
    assert_number_of_arguments_match(func_def_ref, arg_types);    
    size_t number_of_arguments = func_call.arguments.size();
    AssignmentTypeChecker type_checker(type_definitions_register, project_file_structure);
    bool function_is_non_generic_or_it_uses_type_inference = func_call.instantiated_generics.empty();
    bool different_number_of_generics = func_def_ref->template_generics_names.size() != func_call.instantiated_generics.size();
    bool type_parameters_fatal_mismatch = !function_is_non_generic_or_it_uses_type_inference && different_number_of_generics;
    if (type_parameters_fatal_mismatch) {
        return nullptr;
    }
    GenericSubstitutionRule::Set explicit_generics_substitution_rules;
    if (!function_is_non_generic_or_it_uses_type_inference) {
        explicit_generics_substitution_rules  = GenericSubstitutionRule::Set::zip_components_vectors(
            func_def_ref->template_generics_names, 
            func_call.instantiated_generics
        );
    }
    GenericsInstantiationEngine generics_instantiation_engine(explicit_generics_substitution_rules);
    for (size_t i = 0; i < number_of_arguments; i++) {
        TypeSignature declered_arg_type = generics_instantiation_engine.instantiate_generic_typesignature(
            func_def_ref->arguments[i].arg_type
        );
        const TypeSignature& provided_arg_type = arg_types[i];
        if (!type_checker.validate_assignment(provided_arg_type, declered_arg_type)) {
            return nullptr;
        }
    }
    if (function_is_non_generic_or_it_uses_type_inference) {
        if (type_checker.get_generic_substitution_rules()->size() != func_def_ref->template_generics_names.size()) {
            return nullptr;
        }
    }
    return type_checker.get_generic_substitution_rules();
}