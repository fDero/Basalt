//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <optional>
#include <string>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"
#include "language/statements.hpp"


void ensure_use_as_function_argument_complies_with_const_qualifiers(
    const FunctionCall& function_call,
    const Expression& argument,
    bool assignment_discards_qualifiers
) {
    if (!assignment_discards_qualifiers) {
        return;
    }
    throw ValidationError(
        "passing " + describe_expression(argument) + " to " + describe_function_name(function_call) +
        " would allow immutable data to be modified",
        argument.as_debug_informations_aware_entity(),
        "the argument is immutable and its type contains a pointer or a slice, "
        "so the function would get mutable access to the data it refers to"
    );
}

void ensure_assignment_complies_with_const_qualifiers(
    const VariableDeclaration& variable_declaration,
    bool assignment_discards_qualifiers
) {
    if (!assignment_discards_qualifiers) {
        return;
    }
    const Expression& value = variable_declaration.initial_value.value();
    throw ValidationError(
        "initializing variable " + backticked(variable_declaration.identifier_name) + " with " +
        describe_expression(value) + " would allow immutable data to be modified through it",
        value.as_debug_informations_aware_entity(),
        "the value is immutable and its type contains a pointer or a slice; "
        "declare " + backticked(variable_declaration.identifier_name) + " with `const` instead"
    );
}

void ensure_assignment_complies_with_const_qualifiers(
    const Assignment& assignment,
    bool assignment_discards_qualifiers,
    bool target_is_a_constant,
    bool value_gives_access_to_immutable_data
) {
    if (!assignment_discards_qualifiers) {
        return;
    }
    if (target_is_a_constant) {
        throw ValidationError(
            "cannot assign to " + describe_expression(assignment.assignment_target) +
            " because it is a constant",
            assignment.assignment_target.as_debug_informations_aware_entity(),
            "declare it with `var` instead of `const` if it needs to change"
        );
    }
    if (value_gives_access_to_immutable_data) {
        throw ValidationError(
            "assigning " + describe_expression(assignment.assigned_value) + " to " +
            describe_expression(assignment.assignment_target) +
            " would allow immutable data to be modified through it",
            assignment.assigned_value.as_debug_informations_aware_entity(),
            "the value is immutable and its type contains a pointer or a slice, "
            "so after the assignment both would refer to the same memory"
        );
    }
    throw ValidationError(
        "cannot assign to " + describe_expression(assignment.assignment_target) + ": it is immutable",
        assignment.assignment_target.as_debug_informations_aware_entity(),
        "it is part of a constant, or a temporary value such as the result of a function call"
    );
}
