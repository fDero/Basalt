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
#include "language/typesignatures.hpp"
#include "syntax/primitive_types.hpp"


void ensure_typesignature_is_boolean(
    const std::optional<TypeSignature>& operand_type,
    const Expression& operand,
    const std::string& operator_text
) {
    if (operand_type.has_value() && !is_primitive(*operand_type, bool_type)) {
        throw ValidationError(
            "operator " + backticked(operator_text) + " expects operands of type `Bool`, "
            "but " + describe_expression(operand) + " has type " + describe_type(operand_type),
            operand.as_debug_informations_aware_entity()
        );
    }
}

void ensure_typesignature_is_either_numeric_or_generic(
    const std::optional<TypeSignature>& operand_type,
    const Expression& operand,
    const std::string& operator_text
) {
    if (operand_type.has_value() && !operand_type->is<TemplateType>() && !is_numeric(*operand_type)) {
        throw ValidationError(
            "operator " + backticked(operator_text) + " expects numeric operands (`Int` or `Float`), "
            "but " + describe_expression(operand) + " has type " + describe_type(operand_type),
            operand.as_debug_informations_aware_entity()
        );
    }
}

void ensure_numeric_or_generics_types_are_equal(
    const std::optional<TypeSignature>& left_operand_type,
    const std::optional<TypeSignature>& right_operand_type,
    const BinaryOperator& binary_operator
) {
    if (!left_operand_type.has_value() || !right_operand_type.has_value()) {
        return;
    }
    const std::string left = left_operand_type->to_source_string();
    const std::string right = right_operand_type->to_source_string();
    bool both_primitive = left_operand_type->is<PrimitiveType>() && right_operand_type->is<PrimitiveType>();
    bool both_generic = left_operand_type->is<TemplateType>() && right_operand_type->is<TemplateType>();
    if ((!both_primitive && !both_generic) || left != right) {
        throw ValidationError(
            "operands of " + backticked(binary_operator.operator_text) + " have different types: " +
            backticked(left) + " and " + backticked(right),
            binary_operator.as_debug_informations_aware_entity(),
            "Basalt never converts between types implicitly"
        );
    }
}

void ensure_operand_is_a_pointer_for_dereference(
    const TypeSignature& operand_type,
    const UnaryOperator& dereference
) {
    if (!operand_type.is<PointerType>()) {
        throw ValidationError(
            "cannot dereference " + describe_expression(dereference.operand) +
            ": it has type " + describe_type(operand_type) + ", which is not a pointer",
            dereference.as_debug_informations_aware_entity()
        );
    }
}

void ensure_not_trying_to_dereference_a_literal(
    const UnaryOperator& address_of,
    bool is_addressable
) {
    if (!is_addressable) {
        throw ValidationError(
            "cannot take the address of this expression: it is a temporary value, not a variable",
            address_of.as_debug_informations_aware_entity(),
            "only variables, constants, fields and array elements have an address"
        );
    }
}

void ensure_type_is_union_for_type_operator(
    bool is_union,
    const TypeOperator& type_operator,
    const TypeSignature& operand_type
) {
    if (!is_union) {
        throw ValidationError(
            "operator " + backticked(type_operator.operator_text) +
            " can only be applied to values of a union type, "
            "but " + describe_expression(type_operator.expression) +
            " has type " + describe_type(operand_type),
            type_operator.as_debug_informations_aware_entity()
        );
    }
}

void ensure_type_operator_union_operand_can_be_queried_for_the_given_type(
    bool can_hold_queried_type,
    const TypeOperator& type_operator,
    const TypeSignature& operand_type
) {
    if (!can_hold_queried_type) {
        throw ValidationError(
            "a value of type " + describe_type(operand_type) + " can never hold a " +
            describe_type(type_operator.typesignature) + ", so this " +
            backticked(type_operator.operator_text) + " can never succeed",
            type_operator.as_debug_informations_aware_entity()
        );
    }
}

void ensure_typesignature_is_non_string_primitive_or_generic(
    const std::optional<TypeSignature>& operand_type,
    const Expression& operand,
    const std::string& operator_text
) {
    if (!operand_type.has_value() || operand_type->is<TemplateType>()) {
        return;
    }
    if (!operand_type->is<PrimitiveType>() || is_primitive(*operand_type, string_type)) {
        throw ValidationError(
            "operator " + backticked(operator_text) + " cannot compare values of type " +
            describe_type(operand_type), operand.as_debug_informations_aware_entity(),
            "only primitive types other than `String` can be compared with " +
            backticked(operator_text)
        );
    }
}
