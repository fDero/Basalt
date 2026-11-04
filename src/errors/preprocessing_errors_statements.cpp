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
#include "language/statements.hpp"
#include "language/typesignatures.hpp"
#include "syntax/primitive_types.hpp"


void ensure_variable_declaration_is_valid(
    bool assignment_is_valid,
    const VariableDeclaration& variable_declaration,
    const std::optional<TypeSignature>& value_type
) {
    if (assignment_is_valid) {
        return;
    }
    const Expression& value = variable_declaration.initial_value.value();
    throw ValidationError(
        "cannot initialize variable " + backticked(variable_declaration.identifier_name) +
        " of type " + describe_type(variable_declaration.typesignature) +
        " with a value of type " + describe_type(value_type),
        value.as_debug_informations_aware_entity()
    );
}

void ensure_const_declaration_is_valid(
    bool assignment_is_valid,
    const ConstDeclaration& const_declaration,
    const std::optional<TypeSignature>& value_type
) {
    if (assignment_is_valid) {
        return;
    }
    throw ValidationError(
        "cannot initialize constant " + backticked(const_declaration.identifier_name) +
        " of type " + describe_type(const_declaration.typesignature) +
        " with a value of type " + describe_type(value_type),
        const_declaration.value.as_debug_informations_aware_entity()
    );
}

void ensure_assignment_is_valid(
    bool assignment_is_valid,
    const Assignment& assignment,
    const std::optional<TypeSignature>& target_type,
    const std::optional<TypeSignature>& value_type
) {
    if (assignment_is_valid) {
        return;
    }
    throw ValidationError(
        "cannot assign a value of type " + describe_type(value_type) + " to " +
        describe_expression(assignment.assignment_target) + ", which has type " + describe_type(target_type),
        assignment.assigned_value.as_debug_informations_aware_entity()
    );
}

void ensure_condition_expression_is_boolean(
    const std::optional<TypeSignature>& condition_type,
    const Expression& condition
) {
    bool is_boolean = condition_type.has_value()
        && condition_type->is<PrimitiveType>()
        && condition_type->get<PrimitiveType>().type_name == bool_type;
    if (is_boolean) {
        return;
    }
    throw ValidationError(
        "condition must be of type `Bool`, but it has type " + describe_type(condition_type),
        condition.as_debug_informations_aware_entity()
    );
}

void ensure_return_statement_is_congruent_to_function_definition(
    bool return_value_is_expected,
    bool return_value_is_provided,
    const Return& return_statement,
    const std::optional<TypeSignature>& expected_return_type
) {
    if (return_value_is_expected == return_value_is_provided) {
        return;
    }
    if (return_value_is_expected) {
        throw ValidationError(
            "missing return value: this function must return a value of type " + describe_type(expected_return_type),
            return_statement.as_debug_informations_aware_entity()
        );
    }
    throw ValidationError(
        "this function does not return a value, so `return` must not be followed by one",
        return_statement.as_debug_informations_aware_entity(),
        "to return a value, declare a return type: `func name(...) -> Type`"
    );
}

void ensure_return_statement_is_valid(
    bool assignment_is_valid,
    const Return& return_statement,
    const std::optional<TypeSignature>& return_value_type,
    const std::optional<TypeSignature>& expected_return_type
) {
    if (assignment_is_valid) {
        return;
    }
    throw ValidationError(
        "cannot return a value of type " + describe_type(return_value_type) +
        " from a function that returns " + describe_type(expected_return_type),
        return_statement.return_value.value().as_debug_informations_aware_entity()
    );
}
