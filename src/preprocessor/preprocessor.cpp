
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

#include <iostream>

PreProcessor::PreProcessor(
    ProgramRepresentation& program_representation
)
    : project_file_structure(program_representation.project_file_structure)
    , type_definitions_register(program_representation.type_definitions_register)
    , function_overloads_register(program_representation.function_overloads_register)
    , overloading_resolution_engine(program_representation.overloading_resolution_engine)
{}

void PreProcessor::preprocess_packages_typename_conflicts() {
    PackageTypeConflictNavigator package_type_conflict_navigator(project_file_structure);
    for (const auto& files_by_package : project_file_structure.get_all_files_grouped_by_package()) {
        const std::string& package_name = files_by_package.first;
        package_type_conflict_navigator.visit_package(package_name);
    }
}

void PreProcessor::preprocess_type_definitions() {
    TypeDependencyNavigator navigator(type_definitions_register);
    for (const auto& type_definition_wrapper : type_definitions_register.get_all_type_definitions()) {
        const TypeDefinition& type_definition = type_definition_wrapper.second;
        navigator.visit_type_definition(type_definition);
    }
}