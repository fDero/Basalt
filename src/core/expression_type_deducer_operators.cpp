//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <optional>

#include "core/expression_type_deducer.hpp"
#include "errors/internal_errors.hpp"
#include "errors/preprocessing_errors.hpp"
#include "language/expressions.hpp"
#include "language/typesignatures.hpp"
#include "syntax/prefixes.hpp"
#include "syntax/primitive_types.hpp"


std::optional<TypeSignature> ExpressionTypeDeducer::deduce_address_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, address_operator);
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    return (operand_type.has_value())
        ? std::optional<TypeSignature>( PointerType { expression.as_debug_informations_aware_entity(), operand_type.value() } )
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_pointer_dereference_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, pointer_dereference_operator);
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    if (!operand_type.has_value()) {
        return std::nullopt;
    }
    ensure_operand_is_a_pointer_for_dereference(operand_type.value(), expression);
    return operand_type.value().get<PointerType>().pointed_type;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_boolean_not_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, "!");
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    if (!operand_type.has_value()) {
        return std::nullopt;
    }
    ensure_typesignature_is_boolean(operand_type, expression.operand, expression.operator_text);
    return operand_type.value();
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_math_prefix_operator_type(const UnaryOperator& expression) {
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    if (!operand_type.has_value()) {
        return std::nullopt;
    }
    ensure_typesignature_is_either_numeric_or_generic(operand_type, expression.operand, expression.operator_text);
    return operand_type.value(); 
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_eq_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_non_string_primitive_or_generic(left_operand_type, expression.left_operand, expression.operator_text);
    ensure_typesignature_is_non_string_primitive_or_generic(right_operand_type, expression.right_operand, expression.operator_text);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type, expression);
    return deduced
        ? std::optional<TypeSignature>(deduce_primtive_type(bool_type, expression)) 
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_ord_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_either_numeric_or_generic(left_operand_type, expression.left_operand, expression.operator_text);
    ensure_typesignature_is_either_numeric_or_generic(right_operand_type, expression.right_operand, expression.operator_text);
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type, expression);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    return deduced
        ? std::optional<TypeSignature>(deduce_primtive_type(bool_type, expression))
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_logical_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    if (!left_operand_type.has_value() || !right_operand_type.has_value()) {
        return deduce_primtive_type(bool_type, expression);    
    }
    ensure_typesignature_is_boolean(left_operand_type, expression.left_operand, expression.operator_text);
    ensure_typesignature_is_boolean(right_operand_type, expression.right_operand, expression.operator_text);
    return deduce_primtive_type(bool_type, expression);
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_math_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_either_numeric_or_generic(left_operand_type, expression.left_operand, expression.operator_text);
    ensure_typesignature_is_either_numeric_or_generic(right_operand_type, expression.right_operand, expression.operator_text);
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type, expression);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    return deduced
        ? std::optional<TypeSignature>(left_operand_type.value()) 
        : std::nullopt;
}