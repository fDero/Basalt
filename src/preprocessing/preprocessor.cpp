//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/preprocessor.hpp"
#include "preprocessing/package_type_conflict_navigator.hpp"
#include "preprocessing/type_dependency_navigator.hpp"
#include "preprocessing/const_constraint_validator.hpp"
#include "preprocessing/function_definitions_typechecker.hpp"
#include "preprocessing/function_exit_path_navigator.hpp"
#include "preprocessing/address_sanitizer.hpp"
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
    AddressSanitizer address_sanitizer(program_representation);
    FunctionDefinitionsTypeChecker function_definitions_type_checker(program_representation);
    ConstConstraintValidator const_constraint_validator(program_representation);
    FunctionExitPathNavigator function_exit_path_navigator(program_representation);
    address_sanitizer.visit_all_function_definitions();
    function_definitions_type_checker.visit_all_function_definitions();
    const_constraint_validator.visit_all_function_definitions();
    function_exit_path_navigator.visit_all_function_definitions();
}