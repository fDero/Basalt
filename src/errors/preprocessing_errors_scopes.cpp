//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/expressions.hpp"
#include "misc/debug_informations_aware_entity.hpp"


void ensure_identifier_not_ambiguous_in_given_scope(
    const std::string& identifier,
    bool already_declared,
    const DebugInformationsAwareEntity& declaration
) {
    if (already_declared) {
        throw ValidationError(
            backticked(identifier) + " is already declared in this scope",
            declaration
        );
    }
}

void ensure_identifier_is_declared(
    bool is_declared,
    const Identifier& identifier
) {
    if (!is_declared) {
        throw ValidationError(
            "use of undeclared identifier " + backticked(identifier.name),
            identifier.as_debug_informations_aware_entity()
        );
    }
}

void ensure_parent_scope_exists_for_further_local_object_search(
    bool success,
    const std::string& identifier
) {
    if (!success) {
        throw ValidationError(
            "use of undeclared identifier " + backticked(identifier)
        );
    }
}
