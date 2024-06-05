#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "language/generics.hpp"

//------------------------------------- FUNCTION STORE

void ProgramRepresentation::store_function_definition(
    const FunctionDefinition& func_definition, 
    const PackageName& package_name
){
    FunctionDefinitionRef inserted_func_ref = std::make_shared<FunctionDefinition>(func_definition);
    store_function_definition_ref(inserted_func_ref, package_name);
}

void ProgramRepresentation::store_function_definition_ref(
    const FunctionDefinitionRef& func_definition_ref, 
    const PackageName& package_name
){
    const std::string overload_set_id = get_function_definition_overload_set_id(package_name, *func_definition_ref);
    const FunctionSpecificityDescriptor function_specificity(*this, *func_definition_ref);
    function_definitions_specificity.insert({func_definition_ref, function_specificity});
    function_definitions_overload_sets[overload_set_id].push_back(func_definition_ref);
    if (func_definition_ref->template_generics_names.empty()) {
        const std::string retrieve_key = get_function_fast_retrieve_key(package_name, func_definition_ref);
        function_definitions[retrieve_key] = func_definition_ref;
    }
    else {
        const std::string generics_unaware_overload_set_id = get_generics_unaware_function_definition_overload_set_id(package_name, *func_definition_ref);
        function_definitions_overload_sets[generics_unaware_overload_set_id].push_back(func_definition_ref);
    }
}


// ------------------------------------- FUNCTION RETRIEVE

[[nodiscard]] FunctionDefinition ProgramRepresentation::retrieve_func_definition(const PrecompiledFunctionCall& function_call){
    const std::string& target_package = (function_call.package_prefix.empty())
        ? package_name_by_file_name[function_call.filename]
        : function_call.package_prefix;
    std::optional<FunctionDefinitionRef> func_def = search_compatible_function_definitions_within_given_package(function_call, target_package);
    if (func_def.has_value()){
        return *func_def.value();
    }
    for (const FileName& imported_file : imports_by_file[function_call.filename]){
        const PackageName& package_name = package_name_by_file_name[imported_file];
        func_def = search_compatible_function_definitions_within_given_package(function_call, package_name);
        if (func_def.has_value()){
            return *func_def.value();
        }
    }
    throw_no_function_definition_found(function_call);
}

[[nodiscard]] std::optional<ProgramRepresentation::FunctionDefinitionRef> 
ProgramRepresentation::search_compatible_function_definitions_within_given_package(
    const PrecompiledFunctionCall& function_call, 
    const PackageName& package_name
){
    const std::string fast_retrieve_key = get_function_fast_retrieve_key(package_name, function_call);
    if (function_definitions.find(fast_retrieve_key) != function_definitions.end()){
        return function_definitions.at(fast_retrieve_key);
    }
    const std::string overload_set_id = get_function_call_overload_set_id(package_name, function_call);
    std::list<FunctionDefinitionRef> compatible_functions_definitions = 
        search_compatible_function_definitions_within_given_overload_set(function_call, overload_set_id);
    ensure_no_ambiguous_function_definition_found(compatible_functions_definitions, function_call);
    if (compatible_functions_definitions.empty()){
        return std::nullopt;
    }
    const std::string& function_call_source_package_name = (function_call.package_prefix.empty())
        ? package_name_by_file_name[function_call.filename]
        : function_call.package_prefix;
    const std::string newly_creared_fast_retrieve_key = 
        get_function_fast_retrieve_key(function_call_source_package_name, compatible_functions_definitions.front());
    function_definitions[newly_creared_fast_retrieve_key] = compatible_functions_definitions.front();
    return compatible_functions_definitions.front();
} 

[[nodiscard]] std::list<ProgramRepresentation::FunctionDefinitionRef> 
ProgramRepresentation::search_compatible_function_definitions_within_given_overload_set(
    const PrecompiledFunctionCall& precompiled_function_call, 
    const std::string& overload_set_id
){
    FunctionOverloadSet& function_overload_set = function_definitions_overload_sets[overload_set_id];
    std::list<ProgramRepresentation::FunctionDefinitionRef> compatible_functions_definitions;
    std::optional<FunctionSpecificityDescriptor> best_specificity_so_far;
    for (const FunctionDefinitionRef& func_def : function_overload_set){
        if (func_def->arguments.size() != precompiled_function_call.arguments_types.size()){
            continue;
        }
        size_t template_generics_count = func_def->template_generics_names.size();
        size_t instantiated_generics_count = precompiled_function_call.original_function_call.instantiated_generics.size();
        if (template_generics_count != instantiated_generics_count && instantiated_generics_count != 0){
            continue;
        }
        FunctionSpecificityDescriptor::ComparisonOutcome specificity_comparison_outcome = 
            function_definitions_specificity.at(func_def).compare(best_specificity_so_far);
        if (specificity_comparison_outcome == FunctionSpecificityDescriptor::ComparisonOutcome::LESS_SPECIFIC){
            continue;
        }
        std::optional<FunctionDefinitionRef> compatible_func_def = 
            check_function_definition_compatibility(func_def, precompiled_function_call);
        if (!compatible_func_def.has_value()){
            continue;
        }
        if (specificity_comparison_outcome == FunctionSpecificityDescriptor::ComparisonOutcome::MORE_SPECIFIC){
            best_specificity_so_far = function_definitions_specificity.at(func_def);
            compatible_functions_definitions.clear();
        }
        compatible_functions_definitions.push_back(compatible_func_def.value());
    }
    return compatible_functions_definitions;
}

[[nodiscard]] std::optional<ProgramRepresentation::FunctionDefinitionRef>
ProgramRepresentation::check_function_definition_compatibility(
    const FunctionDefinitionRef& function_definition_ref,
    const PrecompiledFunctionCall& precompiled_function_call
){
    size_t function_call_explicit_generics_count = precompiled_function_call.original_function_call.instantiated_generics.size();
    GenericSubstitutionRuleSet substitution_rules;
    if (function_call_explicit_generics_count != 0){
        substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
            function_definition_ref->template_generics_names, 
            precompiled_function_call.original_function_call.instantiated_generics
        );
    }
    AssignmentTypeChecker type_checker(*this);
    bool all_arguments_types_are_compatible = true;
    for (size_t i = 0; i < precompiled_function_call.arguments_types.size(); i++){
        TypeSignature dest = function_definition_ref->arguments[i].arg_type;
        dest.instantiate_generics(substitution_rules);
        const TypeSignature& source = precompiled_function_call.arguments_types[i];
        all_arguments_types_are_compatible &= type_checker.validate_assignment(source, dest);
        if (!all_arguments_types_are_compatible){
            break;
        }
    }
    size_t expected_generics_count = function_definition_ref->template_generics_names.size();
    size_t inferred_generics_count = type_checker.get_generic_substitution_rules().size();
    if (expected_generics_count != inferred_generics_count && function_call_explicit_generics_count == 0){
        return std::nullopt;
    }
    if (!all_arguments_types_are_compatible){
        return std::nullopt;
    }
    if (substitution_rules.size() == 0){
        substitution_rules = type_checker.get_generic_substitution_rules();    
    }
    if (substitution_rules.size() == 0){
        return function_definition_ref;
    }
    FunctionDefinitionRef instantiated_func = std::make_shared<FunctionDefinition>(*function_definition_ref);
    instantiated_func->instantiate_generics(substitution_rules);
    store_function_definition_ref(instantiated_func, precompiled_function_call.package_prefix);
    return instantiated_func;
}



//------------------------------------- FUNCTION FAST RETRIEVE KEY

[[nodiscard]] std::string ProgramRepresentation::get_function_fast_retrieve_key(
    const PackageName& package_name,
    const FunctionDefinitionRef& function_definition_ref
){
    const std::string& func_name = function_definition_ref->function_name;
    const std::vector<FunctionDefinition::Argument>& args = function_definition_ref->arguments;
    std::string function_fast_retrieve_key = package_name + namespace_concatenation + func_name + "(";
    for (const FunctionDefinition::Argument& arg : args){
        function_fast_retrieve_key += get_fully_quilified_typesignature_name(arg.arg_type) + ",";
    }
    if (args.size() > 0){
        function_fast_retrieve_key.back() = ')';
    }
    return function_fast_retrieve_key;
}

[[nodiscard]] std::string ProgramRepresentation::get_function_fast_retrieve_key(
    const PackageName& package_name,
    const PrecompiledFunctionCall& precompiled_function_call
){
    const std::string& func_name = precompiled_function_call.original_function_call.function_name;
    const std::vector<TypeSignature>& arg_types = precompiled_function_call.arguments_types;
    std::string function_fast_retrieve_key = package_name + namespace_concatenation + func_name + "(";
    for (const TypeSignature& arg_type : arg_types){
        function_fast_retrieve_key += get_fully_quilified_typesignature_name(arg_type) + ",";
    }
    if (arg_types.size() > 0){
        function_fast_retrieve_key.back() = ')';
    }
    return function_fast_retrieve_key;
}





//------------------------------------- OVERLOAD SET ID

[[nodiscard]] std::string ProgramRepresentation::get_function_definition_overload_set_id(
    const PackageName& package_name, 
    const FunctionDefinition& function_definition
){
    const std::string& function_name = function_definition.function_name;
    const size_t number_of_args = function_definition.arguments.size();
    const size_t number_of_generics = function_definition.template_generics_names.size();
    const std::string generics_string = "<" + std::to_string(number_of_generics) + ">";
    const std::string args_string = "(" + std::to_string(number_of_args) + ")";
    return package_name + namespace_concatenation + function_name + generics_string + args_string;
}

[[nodiscard]] std::string ProgramRepresentation::get_generics_unaware_function_definition_overload_set_id(
    const PackageName& package_name, 
    const FunctionDefinition& function_definition
){
    const std::string& function_name = function_definition.function_name;
    const size_t number_of_args = function_definition.arguments.size();
    const std::string args_string = "(" + std::to_string(number_of_args) + ")";
    return package_name + namespace_concatenation + function_name + "<0>" + args_string;
}

[[nodiscard]] std::string ProgramRepresentation::get_function_call_overload_set_id(
    const PackageName& package_name, 
    const PrecompiledFunctionCall& precompiled_function_call
){
    const std::string& function_name = precompiled_function_call.original_function_call.function_name;
    const std::vector<TypeSignature>& arguments_types = precompiled_function_call.arguments_types;
    const ConcreteGenerics& genenerics = precompiled_function_call.original_function_call.instantiated_generics;
    const std::string generics_string = "<" + std::to_string(genenerics.size()) + ">";
    const std::string args_string = "(" + std::to_string(arguments_types.size()) + ")";
    std::string overload_set_id = package_name + namespace_concatenation + function_name + generics_string + args_string;
    return overload_set_id;
}