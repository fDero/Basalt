
#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "toolchain/preprocessor.hpp"
#include "language/generics.hpp"

#include <iostream>

[[nodiscard]] std::optional<FunctionDefinition> ProgramRepresentation::search_compatible_function_definitions_within_given_package(
    const PrecompiledFunctionCall& function_call, 
    const PackageName& package_name
){
    const std::string& function_name = function_call.original_function_call.function_name;
    const std::vector<TypeSignature>& arguments_types = function_call.arguments_types;
    const std::string fast_retrieve_key = get_function_fast_retrieve_key(package_name, function_name, arguments_types);
    if (function_definitions.find(fast_retrieve_key) != function_definitions.end()){
        const FunctionDefinitionFastRetrieveKey& fast_retrieve_key_data = function_definitions[fast_retrieve_key];
        const std::string& overload_set_id = fast_retrieve_key_data.overload_set_id;
        const size_t index = fast_retrieve_key_data.index_within_overload_set;       
        return function_definitions_overload_sets[overload_set_id][index];
    }
    const std::string overload_set_id = get_function_call_overload_set_id(package_name, function_call);
    const FunctionOverloadSet& overload_set = function_definitions_overload_sets[overload_set_id];
    std::vector<FunctionDefinition> compatible_functions_definitions = 
        search_compatible_function_definitions_within_given_overload_set(function_call, overload_set);
    ensure_no_ambiguous_function_definition_found(compatible_functions_definitions, function_call);
    return (!compatible_functions_definitions.empty())
        ? std::optional<FunctionDefinition>(compatible_functions_definitions.back())
        : std::nullopt;
} 

[[nodiscard]] FunctionDefinition ProgramRepresentation::retrieve_func_definition(const PrecompiledFunctionCall& function_call){
    const std::string& target_package = (function_call.package_prefix.empty())
        ? package_name_by_file_name[function_call.filename]
        : function_call.package_prefix;
    std::optional<FunctionDefinition> func_def = search_compatible_function_definitions_within_given_package(function_call, target_package);
    if (func_def.has_value()){
        return func_def.value();
    }
    for (const FileName& imported_file : imports_by_file[function_call.filename]){
        const PackageName& package_name = package_name_by_file_name[imported_file];
        func_def = search_compatible_function_definitions_within_given_package(function_call, package_name);
        if (func_def.has_value()){
            return func_def.value();
        }
    }
    throw_no_function_definition_found(function_call);
}

[[nodiscard]] std::vector<FunctionDefinition> ProgramRepresentation::search_compatible_function_definitions_within_given_overload_set(
    const PrecompiledFunctionCall& precompiled_function_call, 
    const FunctionOverloadSet& function_overload_set
){
    size_t generics_so_far = std::numeric_limits<size_t>::max();
    std::vector<FunctionDefinition> compatible_functions_definitions;
    for (const FunctionDefinition& func_def : function_overload_set){
        if (func_def.template_generics_names.size() > generics_so_far){
            continue;
        }
        if (func_def.arguments.size() != precompiled_function_call.arguments_types.size()){
            continue;
        }
        size_t template_generics_count = func_def.template_generics_names.size();
        size_t instantiated_generics_count = precompiled_function_call.original_function_call.instantiated_generics.size();
        if (template_generics_count != instantiated_generics_count && instantiated_generics_count != 0){
            continue;
        }
        GenericSubstitutionRuleSet substitution_rules;
        if (instantiated_generics_count != 0){
            substitution_rules = GenericSubstitutionRuleSet::zip_components_vectors(
                func_def.template_generics_names, 
                precompiled_function_call.original_function_call.instantiated_generics
            );
        }
        AssignmentTypeChecker type_checker(*this);
        bool all_arguments_types_are_compatible = true;
        for (size_t i = 0; i < precompiled_function_call.arguments_types.size(); i++){
            TypeSignature dest = func_def.arguments[i].arg_type;
            dest.instantiate_generics(substitution_rules);
            const TypeSignature& source = precompiled_function_call.arguments_types[i];
            all_arguments_types_are_compatible &= type_checker.validate_assignment(source, dest);
            if (!all_arguments_types_are_compatible){
                break;
            }
        }
        if (all_arguments_types_are_compatible){
            if (func_def.template_generics_names.size() < generics_so_far){
                compatible_functions_definitions.clear();
                generics_so_far = func_def.template_generics_names.size();
            }
            compatible_functions_definitions.push_back(func_def);
        }
    }
    return compatible_functions_definitions;
}

void ProgramRepresentation::store_function_definition(
    const FunctionDefinition& func_definition, 
    const PackageName& package_name
){
    size_t index_of_insertion = function_definitions_overload_sets.size();
    const std::string overload_set_id = get_function_definition_overload_set_id(package_name, func_definition);
    function_definitions_overload_sets[overload_set_id].push_back(func_definition);
    if (func_definition.template_generics_names.empty()) {
        const std::string func_name = func_definition.function_name;
        std::vector<TypeSignature> arguments_types;
        for (const FunctionDefinition::Argument& arg : func_definition.arguments){
            arguments_types.push_back(arg.arg_type);
        }
        const std::string retrieve_key = get_function_fast_retrieve_key(package_name, func_name, arguments_types);
        function_definitions[retrieve_key] = FunctionDefinitionFastRetrieveKey {overload_set_id, index_of_insertion};
    }
    else {
        const std::string generics_unaware_overload_set_id = get_generics_unaware_function_definition_overload_set_id(package_name, func_definition);
        function_definitions_overload_sets[generics_unaware_overload_set_id].push_back(func_definition);
    }
}

[[nodiscard]] std::string ProgramRepresentation::get_function_fast_retrieve_key(
    const PackageName& package_name,
    const std::string& func_name, 
    const std::vector<TypeSignature>& arguments_types
){
    std::string function_fast_retrieve_key = package_name + namespace_concatenation + func_name + "(";
    for (const TypeSignature& arg_type : arguments_types){
        function_fast_retrieve_key += get_fully_quilified_typesignature_name(arg_type) + ",";
    }
    if (arguments_types.size() > 0){
        function_fast_retrieve_key.back() = ')';
    }
    return function_fast_retrieve_key;
}

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
