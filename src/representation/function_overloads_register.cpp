
#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "language/generics.hpp"

FunctionOverloadsRegister::FunctionOverloadsRegister(ProjectFileStructure& project_file_structure) 
    : project_file_structure(project_file_structure) 
{ 
    for (const auto& [package_name, files] : project_file_structure.get_all_files_grouped_by_package()) {
        for (const auto& file : files) {
            for (const auto& func_def : file.func_defs) {
                store_function_definition(func_def, package_name);
            }
        }
    }
}

void FunctionOverloadsRegister::store_function_definition(
    const FunctionDefinition& func_def,
    const std::string& package_name
) {
    const std::string overload_set_id = get_function_definition_overload_set_id(package_name, func_def);
    function_definitions_overload_sets[overload_set_id].push_back(func_def);
    if (!func_def.template_generics_names.empty()) {
        const std::string generics_unaware_overload_set_id = get_generics_unaware_function_definition_overload_set_id(package_name, func_def);
        function_definitions_overload_sets[generics_unaware_overload_set_id].push_back(func_def);
    }
}

[[nodiscard]] std::string FunctionOverloadsRegister::get_function_definition_overload_set_id(
    const std::string& package_name, 
    const FunctionDefinition& function_definition
) {
    const std::string& function_name = function_definition.function_name;
    const size_t number_of_args = function_definition.arguments.size();
    const size_t number_of_generics = function_definition.template_generics_names.size();
    const std::string generics_string = "<" + std::to_string(number_of_generics) + ">";
    const std::string args_string = "(" + std::to_string(number_of_args) + ")";
    return package_name + namespace_concatenation + function_name + generics_string + args_string;
}

[[nodiscard]] std::string FunctionOverloadsRegister::get_generics_unaware_function_definition_overload_set_id(
    const std::string& package_name, 
    const FunctionDefinition& function_definition
) {
    const std::string& function_name = function_definition.function_name;
    const size_t number_of_args = function_definition.arguments.size();
    const std::string args_string = "(" + std::to_string(number_of_args) + ")";
    return package_name + namespace_concatenation + function_name + "<0>" + args_string;
}

[[nodiscard]] std::string FunctionOverloadsRegister::get_function_call_overload_set_id(
    const std::string& package_name, 
    const FunctionCall& original_function_call
) {
    const std::string& function_name = original_function_call.function_name;
    const std::vector<TypeSignature>& generics = original_function_call.instantiated_generics;
    const std::string generics_string = "<" + std::to_string(generics.size()) + ">";
    const std::string args_string = "(" + std::to_string(original_function_call.arguments.size()) + ")";
    std::string overload_set_id = package_name + namespace_concatenation + function_name + generics_string + args_string;
    return overload_set_id;
}

[[nodiscard]] std::vector<std::string> FunctionOverloadsRegister::retrieve_overload_sets_ids(const FunctionCall& function_call) {
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

[[nodiscard]] std::vector<FunctionDefinition>& FunctionOverloadsRegister::retrieve_specific_overload_set(const std::string& overload_set_id) {
    auto overload_set_search_outcome = function_definitions_overload_sets.find(overload_set_id);
    assert_overload_set_exists(overload_set_search_outcome, function_definitions_overload_sets.end());
    return overload_set_search_outcome->second;
}