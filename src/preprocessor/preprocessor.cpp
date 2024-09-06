
#include "preprocessing/preprocessor.hpp"
#include "preprocessing/package_type_conflict_navigator.hpp"
#include "preprocessing/function_definition_validator.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

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
    project_file_structure.foreach_package([&](const std::string& package_name) {
        package_type_conflict_navigator.visit_package(package_name);
    });
}

void PreProcessor::preprocess_type_definitions() {
    TypeDependencyNavigator type_dependency_navigator(type_definitions_register);
    type_dependency_navigator.visit_all_type_definitions();
}

void PreProcessor::preprocess_function_definitions() {
    FunctionDefinitionValidator function_definition_validator(
        project_file_structure,
        type_definitions_register,
        function_overloads_register,
        overloading_resolution_engine
    );
    function_overloads_register.foreach_function_definition([&](const FunctionDefinition::Ref& function_definition) {
        function_definition_validator.validate_function_definition(*function_definition);
    });
}