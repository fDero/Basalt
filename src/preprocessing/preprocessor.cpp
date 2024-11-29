//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/preprocessor.hpp"
#include "preprocessing/package_type_conflict_navigator.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "preprocessing/function_exit_path_navigator.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

PreProcessor::PreProcessor(
    ProgramRepresentation& program_representation
)
    : program_representation(program_representation)
{}

void PreProcessor::preprocess_packages_typename_conflicts() {
    PackageTypeConflictNavigator package_type_conflict_navigator(program_representation);
    package_type_conflict_navigator.visit_all_packages();
}

void PreProcessor::preprocess_type_definitions() {
    TypeDependencyNavigator type_dependency_navigator(program_representation);
    type_dependency_navigator.visit_all_type_definitions();
}

void PreProcessor::preprocess_function_definitions() {
    FunctionExitPathNavigator function_exit_path_navigator(program_representation);
    function_exit_path_navigator.visit_all_function_definitions();
}