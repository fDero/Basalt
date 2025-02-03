//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/expression_type_deducer.hpp"
#include "core/assignment_type_checker.hpp"
#include "frontend/syntax.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

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
    ensure_typesignature_is<PointerType>(operand_type.value());
    return operand_type.value().get<PointerType>().pointed_type;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_boolean_not_operator_type(const UnaryOperator& expression) {
    assert_unary_operator_is(expression, "!");
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    if (!operand_type.has_value()) {
        return std::nullopt;
    }
    ensure_typesignature_is_boolean(operand_type);
    return operand_type.value();
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_math_prefix_operator_type(const UnaryOperator& expression) {
    std::optional<TypeSignature> operand_type = deduce_expression_type(expression.operand);
    if (!operand_type.has_value()) {
        return std::nullopt;
    }
    ensure_typesignature_is_either_numeric_or_generic(operand_type.value());
    return operand_type.value(); 
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_eq_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_non_string_primitive_or_generic(left_operand_type);
    ensure_typesignature_is_non_string_primitive_or_generic(right_operand_type);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type);
    return deduced
        ? std::optional<TypeSignature>(deduce_primtive_type("Bool", expression)) 
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_ord_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_either_numeric_or_generic(left_operand_type);
    ensure_typesignature_is_either_numeric_or_generic(right_operand_type);
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    return deduced
        ? std::optional<TypeSignature>(deduce_primtive_type("Bool", expression))
        : std::nullopt;
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_logical_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    if (!left_operand_type.has_value() || !right_operand_type.has_value()) {
        return deduce_primtive_type("Bool", expression);    
    }
    AssignmentTypeChecker assignment_type_checker(type_definitions_register, project_file_structure);
    ensure_typesignature_is_boolean(left_operand_type);
    ensure_typesignature_is_boolean(left_operand_type);
    return deduce_primtive_type("Bool", expression);
}

std::optional<TypeSignature> ExpressionTypeDeducer::deduce_type_from_math_binary_operator(const BinaryOperator& expression) {
    std::optional<TypeSignature> left_operand_type = deduce_expression_type(expression.left_operand);
    std::optional<TypeSignature> right_operand_type = deduce_expression_type(expression.right_operand);
    ensure_typesignature_is_either_numeric_or_generic(left_operand_type);
    ensure_typesignature_is_either_numeric_or_generic(right_operand_type);
    ensure_numeric_or_generics_types_are_equal(left_operand_type, right_operand_type);
    bool deduced = left_operand_type.has_value() && right_operand_type.has_value();
    return deduced
        ? std::optional<TypeSignature>(left_operand_type.value()) 
        : std::nullopt;
}