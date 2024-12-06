//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/caching_aware_register.hpp"

std::string CachingAwareRegister::get_cache_search_key_for_func_def_retrieval_from_func_call(
    TypeDefinitionsRegister& type_definitions_register,
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
) {
    std::string search_key = function_call.as_debug_informations_aware_entity().filename;
    if (!function_call.package_prefix.empty()) {
        search_key = function_call.package_prefix;
    }
    search_key += namespace_concatenation + function_call.function_name;
    search_key += "<" + std::to_string(function_call.instantiated_generics.size()) + ">";
    for (const TypeSignature& arg_type : arg_types) {
        search_key += type_definitions_register.get_fully_qualified_typesignature_name(arg_type);
        search_key += ";";
    }
    return search_key;
}

std::string CachingAwareRegister::get_function_definition_overload_set_id(
    const std::string& package_name, 
    const FunctionDefinition::Ref function_definition_ref
) {
    const std::string& function_name = function_definition_ref->function_name;
    const size_t number_of_args = function_definition_ref->arguments.size();
    const size_t number_of_generics = function_definition_ref->template_generics_names.size();
    const std::string generics_string = "<" + std::to_string(number_of_generics) + ">";
    const std::string args_string = "(" + std::to_string(number_of_args) + ")";
    return package_name + namespace_concatenation + function_name + generics_string + args_string;
}

std::string CachingAwareRegister::get_generics_unaware_function_definition_overload_set_id(
    const std::string& package_name, 
    const FunctionDefinition::Ref function_definition_ref
) {
    const std::string& function_name = function_definition_ref->function_name;
    const size_t number_of_args = function_definition_ref->arguments.size();
    const std::string args_string = "(" + std::to_string(number_of_args) + ")";
    return package_name + namespace_concatenation + function_name + "<0>" + args_string;
}

std::string CachingAwareRegister::get_function_call_overload_set_id(
    const std::string& package_name, 
    const FunctionCall& original_function_call
) {
    const std::string& actual_package_prefix = original_function_call.package_prefix.empty()? 
        package_name : original_function_call.package_prefix;
    const std::string& function_name = original_function_call.function_name;
    const std::vector<TypeSignature>& generics = original_function_call.instantiated_generics;
    const std::string generics_string = "<" + std::to_string(generics.size()) + ">";
    const std::string args_string = "(" + std::to_string(original_function_call.arguments.size()) + ")";
    std::string overload_set_id = actual_package_prefix + namespace_concatenation + function_name + generics_string + args_string;
    return overload_set_id;
}

std::string CachingAwareRegister::get_overload_set_default_search_key(
    ProjectFileStructure project_file_structure, 
    const FunctionCall& function_call
) {
    if (!function_call.package_prefix.empty()) {
        return get_function_call_overload_set_id(function_call.package_prefix, function_call);
    }
    const std::string& file_where_function_call_is_located = 
        function_call.as_debug_informations_aware_entity().filename;
    const std::string& package_where_the_function_call_is_located = 
        project_file_structure.get_package_name_by_file_name(file_where_function_call_is_located);
    return get_function_call_overload_set_id(package_where_the_function_call_is_located, function_call);
}