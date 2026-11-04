//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/generics_substitution_rules.hpp"
#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "language/typesignatures.hpp"


void ensure_no_multiple_ambiguous_candidate_function_overloads_have_been_found(
    const std::vector<std::pair<FunctionDefinition::Ref, GenericSubstitutionRule::Set::Ref>>& best_matches,
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
) {
    std::string candidates;
    for (const auto& [candidate, _] : best_matches) {
        candidates += "\n\tcandidate defined at " +
        describe_location(candidate->as_debug_informations_aware_entity());
    }
    if (best_matches.size() >= 2) {
        throw ValidationError(
            "call to " + describe_function_name(function_call) + " with argument types " +
            describe_argument_types(arg_types) + " is ambiguous: " +
            std::to_string(best_matches.size()) + " overloads match equally well",
            function_call.as_debug_informations_aware_entity(),
            "the matching overloads are:" + candidates
        );
    }
}

void ensure_common_feature_adoption_is_possible(
    std::vector<TypeSignature>::const_iterator current_arg_type_iterator,
    std::vector<TypeSignature>::const_iterator arg_types_end,
    const FunctionCall& function_call,
    const std::vector<TypeSignature>& arg_types
) {
    if (current_arg_type_iterator == arg_types_end) {
        throw IndexingError(
            "no overload of " + describe_function_name(function_call) +
            " accepts argument types " + describe_argument_types(arg_types),
            function_call.as_debug_informations_aware_entity(),
            "check the number and the types of the arguments; if an argument is a union, "
            "every type it can hold needs a matching overload"
        );
    }
}

void ensure_common_feature_adoption_is_possible(
    const FunctionCall& function_call,
    bool contains_void_return_type,
    bool contains_non_void_return_type
) {
    if (contains_void_return_type && contains_non_void_return_type) {
        throw ValidationError(
            "cannot call " + describe_function_name(function_call) + " on a union argument: "
            "some of the overloads it dispatches to return a value and others don't",
            function_call.as_debug_informations_aware_entity(),
            "every overload selected for the types the union can hold must agree on whether it returns a value"
        );
    }
}

void ensure_function_has_a_return_type(
    const FunctionCall& function_call,
    const std::optional<TypeSignature>& return_type
) {
    if (!return_type.has_value()) {
        throw ValidationError(
            describe_function_name(function_call) +
            " does not return a value, so it cannot be used as an expression",
            function_call.as_debug_informations_aware_entity()
        );
    }
}

void ensure_return_value_of_function_is_treated_correctly_during_fcall(
    const FunctionCall& function_call,
    bool should_return_something,
    bool is_void
) {
    if (should_return_something != is_void) {
        return;
    }
    if (should_return_something) {
        throw ValidationError(
            describe_function_name(function_call) +
            " does not return a value, so it cannot be used as an expression",
            function_call.as_debug_informations_aware_entity()
        );
    }
    throw ValidationError(
        "the value returned by " + describe_function_name(function_call) + " is ignored",
        function_call.as_debug_informations_aware_entity(),
        "Basalt does not allow discarding return values; store it in a variable"
    );
}
