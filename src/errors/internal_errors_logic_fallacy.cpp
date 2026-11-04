//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cstring>
#include <string>
#include <typeinfo>
#include <vector>

#include "errors/compilation_error.hpp"
#include "errors/internal_errors.hpp"
#include "language/definitions.hpp"
#include "language/typesignatures.hpp"


[[noreturn]] void throw_internal_error(const std::string& message) {
    throw InternalError(message);
}

[[noreturn]] void assert_unreachable() {
    throw_internal_error("an execution path that should have been unreachable has been reached");
}

void assert_get_operation_is_possible(const char* wanted_type, const std::type_info& type_info) {
    #ifdef DEBUG_BUILD
    // type names must be compared by content: the same name is not guaranteed
    // to live at the same address (e.g. across shared libraries or with some compilers)
    if (std::strcmp(wanted_type, type_info.name()) != 0) {
        throw_internal_error("an attempt has been made to cast something that was holding "
            "the type: " + std::string(wanted_type) + " to the type: " + type_info.name());
    }
    #endif
}

void assert_number_of_arguments_match(
    const FunctionDefinition::Ref& function_definition,
    const std::vector<TypeSignature>& arg_types
) {
    // not guarded by DEBUG_BUILD: the overload resolution indexes `arg_types`
    // by argument position, so a mismatch here would be an out-of-bounds read
    if (function_definition->arguments.size() != arg_types.size()) {
        throw_internal_error(
            "overload resolution for `" + function_definition->function_name + "` was attempted with " +
            std::to_string(arg_types.size()) + " argument types, but the function takes " +
            std::to_string(function_definition->arguments.size()) + " arguments"
        );
    }
}

void assert_function_name_is_main(const FunctionDefinition::Ref& main_function_definition) {
    if (main_function_definition->function_name != "main") {
        throw_internal_error(
            "`" + main_function_definition->function_name + "` was registered as a main function"
        );
    }
}
