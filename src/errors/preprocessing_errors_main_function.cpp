//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <optional>
#include <string>
#include <vector>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/definitions.hpp"
#include "language/typesignatures.hpp"
#include "syntax/primitive_types.hpp"


void ensure_not_multiple_main_functions_have_been_found(
    const std::vector<FunctionDefinition::Ref>& main_function_definitions
) {
    if (main_function_definitions.size() >= 2) {
        throw ValidationError(
            "the program defines more than one `main` function",
            main_function_definitions[1]->as_debug_informations_aware_entity(),
            "the first one is defined at " + describe_location(
                main_function_definitions[0]->as_debug_informations_aware_entity()
            )
        );
    }
}

void ensure_main_function_exists(
    const std::vector<FunctionDefinition::Ref>& main_function_definitions
) {
    if (main_function_definitions.empty()) {
        throw ValidationError(
            "no `main` function found: a program needs a `func main()` in package `main`"
        );
    }
}

void ensure_main_function_is_not_generic(
    const FunctionDefinition::Ref& main_function_definition
) {
    if (!main_function_definition->template_generics_names.empty()) {
        throw ValidationError(
            "the `main` function cannot be generic",
            main_function_definition->as_debug_informations_aware_entity()
        );
    }
}

void ensure_main_function_has_no_arguments(
    const FunctionDefinition::Ref& main_def
) {
    if (!main_def->arguments.empty()) {
        throw ValidationError(
            "the `main` function cannot take arguments",
            main_def->as_debug_informations_aware_entity()
        );
    }
}

void ensure_main_function_returns_either_int_or_void(
    const FunctionDefinition::Ref& main_def
) {
    const std::optional<TypeSignature>& return_type = main_def->return_type;
    bool returns_int = return_type.has_value()
        && return_type->is<PrimitiveType>()
        && return_type->get<PrimitiveType>().type_name == int_type;
    if (!return_type.has_value() || returns_int) {
        return;
    }
    throw ValidationError(
        "the `main` function must return either nothing or `Int`, not " +
        describe_type(return_type), main_def->as_debug_informations_aware_entity()
    );
}

void ensure_main_function_is_in_main_package(
    const FunctionDefinition::Ref& main_def,
    const std::string& package_name
) {
    if (package_name == "main") {
        return;
    }
    throw ValidationError(
        "the `main` function must be in package `main`, not in package " +
        backticked(package_name), main_def->as_debug_informations_aware_entity()
    );
}
