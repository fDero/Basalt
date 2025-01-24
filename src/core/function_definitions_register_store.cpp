//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"
#include "core/function_definitions_register.hpp"
#include "core/function_definitions_register.hpp"

void FunctionDefinitionsRegister::store_function_definition(
    const FunctionDefinition& func_def
) {
    const std::string package_name = project_file_structure.get_package_name_by_file_name(func_def.filename);
    FunctionDefinition::Ref func_def_ref = std::make_shared<FunctionDefinition>(func_def);
    maybe_register_as_main_function(func_def_ref, package_name);
    function_definitions.push_back(func_def_ref);
    const std::string overload_set_id = get_function_definition_overload_set_id(package_name, func_def_ref);
    function_definitions_overload_sets[overload_set_id].push_back(func_def_ref);
    if (!func_def.template_generics_names.empty()) {
        const std::string generics_unaware_overload_set_id = get_generics_unaware_function_definition_overload_set_id(package_name, func_def_ref);
        function_definitions_overload_sets[generics_unaware_overload_set_id].push_back(func_def_ref);
    }
}

std::vector<std::string> FunctionDefinitionsRegister::retrieve_overload_sets_ids(const FunctionCall& function_call) {
    if (!function_call.package_prefix.empty()) {
        std::string overload_set_id = get_function_call_overload_set_id(function_call.package_prefix, function_call); 
        return { overload_set_id };
    }
    std::vector<std::string> overload_sets_ids;
    const std::string& file_where_function_call_is_located = function_call.as_debug_informations_aware_entity().filename;
    const std::string& package_where_the_function_call_is_located = project_file_structure.get_package_name_by_file_name(file_where_function_call_is_located);
    std::vector<std::string> packages_to_explore { package_where_the_function_call_is_located };
    for (const auto& imported_package : project_file_structure.get_imports_by_file(file_where_function_call_is_located)) {
        packages_to_explore.push_back(imported_package);
    }
    for (const std::string& package_name : packages_to_explore) {
        const std::string overload_set_id = get_function_call_overload_set_id(package_name, function_call);
        auto overload_set_search_outcome = function_definitions_overload_sets.find(overload_set_id);
        if (overload_set_search_outcome != function_definitions_overload_sets.end()) {
            overload_sets_ids.push_back(overload_set_id);
        }
    }
    return overload_sets_ids;
}

std::vector<FunctionDefinition::Ref>& FunctionDefinitionsRegister::retrieve_specific_overload_set(const std::string& overload_set_id) {
    static std::vector<FunctionDefinition::Ref> empty_overload_set;
    auto overload_set_search_outcome = function_definitions_overload_sets.find(overload_set_id);
    if (overload_set_search_outcome == function_definitions_overload_sets.end()) {
        return empty_overload_set;
    }
    return overload_set_search_outcome->second;
}