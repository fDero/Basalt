//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>
#include <vector>

#include "core/program_representation.hpp"
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/definitions.hpp"
#include "preprocessing/main_functions_preprocessor.hpp"


MainFunctionsPreprocessor::MainFunctionsPreprocessor(ProgramRepresentation& program_representation)
    : program_representation(program_representation)
{ }

void MainFunctionsPreprocessor::preprocess_all_main_functions() {
    std::vector<FunctionDefinition::Ref> main_function_definitions;
    program_representation.foreach_main_function_definition([&](
        const FunctionDefinition::Ref& main_function_definition, 
        const std::string& package_name
    ) {
        main_function_definitions.push_back(main_function_definition);
        preprocess_main_function(main_function_definition, package_name);
    });
    ensure_not_multiple_main_functions_have_been_found(main_function_definitions);
}

void MainFunctionsPreprocessor::ensure_program_has_a_main_function() {
    std::vector<FunctionDefinition::Ref> main_function_definitions;
    program_representation.foreach_main_function_definition([&](
        const FunctionDefinition::Ref& main_function_definition, 
        const std::string&
    ) {
        main_function_definitions.push_back(main_function_definition);
    });
    ensure_main_function_exists(main_function_definitions);
}

void MainFunctionsPreprocessor::preprocess_main_function(
    const FunctionDefinition::Ref& main_function_definition,
    const std::string& package_name
) {
    assert_function_name_is_main(main_function_definition);
    ensure_main_function_is_not_generic(main_function_definition);
    ensure_main_function_has_no_arguments(main_function_definition);
    ensure_main_function_returns_either_int_or_void(main_function_definition);
    ensure_main_function_is_in_main_package(main_function_definition, package_name);
}
