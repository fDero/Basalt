//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <unordered_set>

#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/definitions.hpp"

void assert_number_of_arguments_match(
    FunctionDefinition::Ref function_definition,
    const std::vector<TypeSignature>& arg_types
) {
    #ifdef DEBUG
    if (function_definition->arguments.size() != arg_types.size()) {
        throw InternalError("number of arguments does not match");
    }
    #endif
}