//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/main_functions_preprocessor.hpp"
#include "errors/preprocessing_errors.hpp"

MainFunctionsPreprocessor::MainFunctionsPreprocessor(ProgramRepresentation& program_representation)
    : program_representation(program_representation)
{ }

void MainFunctionsPreprocessor::preprocess_all_main_functions() {
    size_t main_function_counter = 0;
    program_representation.foreach_main_function_definition([&](
        const FunctionDefinition::Ref& main_function_definition, 
        const std::string& package_name
    ) {
        ++main_function_counter;
        preprocess_main_function(main_function_definition, package_name);
    });
    ensure_not_multiple_main_functions_have_been_found(main_function_counter);
}

void MainFunctionsPreprocessor::preprocess_main_function(
    const FunctionDefinition::Ref& main_function_definition,
    const std::string& package_name
) {
    assert_function_name_is_main(main_function_definition);
    ensure_main_function_is_not_generic(main_function_definition);
    ensure_main_function_has_no_arguments(main_function_definition);
    ensure_main_function_returns_either_int_or_void(main_function_definition);
    ensure_main_function_is_in_main_package(package_name);
}