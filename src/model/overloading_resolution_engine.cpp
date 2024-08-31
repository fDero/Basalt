
#include "model/function_specificity_descriptor.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "language/generics.hpp"
#include "preprocessing/preprocessor.hpp"
#include "typechecking/assignment_type_checker.hpp"

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
    std::string function_call_default_search_key = function_overloads_register.get_function_default_search_key(function_call);
    auto cache_search_outcome = fast_retrieve_cache.find(function_call_default_search_key);
    if (cache_search_outcome != fast_retrieve_cache.end()) {
        return cache_search_outcome->second;
    }
    FunctionDefinition::Ref func_def_ref = cache_unaware_function_definition_retrieval(function_call, arg_types);
    fast_retrieve_cache.insert({function_call_default_search_key, func_def_ref});
    return func_def_ref;
}

FunctionDefinition::Ref OverloadingResolutionEngine::cache_unaware_function_definition_retrieval(
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
) {
    std::vector<std::string> candidate_overload_set_ids = function_overloads_register.retrieve_overload_sets_ids(function_call);
    std::vector<std::pair<FunctionDefinition::Ref, GenericSubstitutionRuleSet::Ref>> best_maches_so_far; 
    FunctionSpecificityDescriptor best_specificity_so_far = FunctionSpecificityDescriptor::worst_possible_specificity();
    for (const std::string& overload_set_id : candidate_overload_set_ids) {
        std::vector<FunctionDefinition::Ref>& current_overload_set = function_overloads_register.retrieve_specific_overload_set(overload_set_id);
        for (const FunctionDefinition::Ref& func_def_ref : current_overload_set) {
            FunctionSpecificityDescriptor current_specificity(*func_def_ref, type_definitions_register);
            FunctionSpecificityDescriptor::ComparisonResult comparison_result = current_specificity.compare_with(best_specificity_so_far);
            bool is_less_specific = comparison_result == FunctionSpecificityDescriptor::ComparisonResult::less_specific;
            GenericSubstitutionRuleSet::Ref generic_substitution_rules_ref = check_function_compatibility(func_def_ref, function_call, arg_types);
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
    ensure_no_multiple_ambiguous_candidate_function_overloads_have_been_found(best_maches_so_far);
    if (best_maches_so_far.empty()) {
        return nullptr;
    }
    const FunctionDefinition& best_match = *best_maches_so_far[0].first; 
    GenericsInstantiationEngine generics_instantiation_engine(*best_maches_so_far[0].second);
    FunctionDefinition::Ref instanitated_func_def_ref = 
        generics_instantiation_engine.instantiate_generic_function(best_match);
    return instanitated_func_def_ref;
}

GenericSubstitutionRuleSet::Ref OverloadingResolutionEngine::check_function_compatibility(
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
    GenericSubstitutionRuleSet explicit_generics_substitution_rules;
    if (!function_is_non_generic_or_it_uses_type_inference) {
        explicit_generics_substitution_rules  = GenericSubstitutionRuleSet::zip_components_vectors(
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