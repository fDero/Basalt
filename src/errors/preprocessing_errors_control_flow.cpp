//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/definitions.hpp"
#include "language/statements.hpp"


void ensure_every_path_returns_in_function_exit_navigation(
    bool every_path_returns,
    const FunctionDefinition& function_definition
) {
    if (!every_path_returns) {
        throw ValidationError(
            "function " + backticked(function_definition.function_name) +
            " does not return a value on every path",
            function_definition.as_debug_informations_aware_entity(),
            "it is declared to return " + describe_type(function_definition.return_type) +
            ", so every path through its body must end with a `return`"
        );
    }
}

void ensure_no_unreachable_code_in_function_exit_navigation(
    bool code_from_now_on_is_unreachable,
    const Statement& statement
) {
    if (code_from_now_on_is_unreachable) {
        throw ValidationError(
            "unreachable code: this statement can never be executed",
            statement.as_debug_informations_aware_entity(),
            "it comes after a `return`, `break` or `continue` in the same block"
        );
    }
}

void ensure_no_loop_specific_statements_outside_loop_body_in_function_exit_navigation(
    bool inside_loop,
    const Statement& statement
) {
    if (!inside_loop) {
        std::string keyword = statement.is<Break>() ? "break" : "continue";
        throw ValidationError(
            backticked(keyword) + " can only be used inside a loop",
            statement.as_debug_informations_aware_entity()
        );
    }
}
