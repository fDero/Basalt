
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] Expression Parser::rotate_binary_operator_to_match_operators_priority(const BinaryOperator& binary_operator) {
    if (!binary_operator.left_operand.is_wrapped_in_in_parenthesis()) {
        if (binary_operator.left_operand.is<BinaryOperator>()) {
            return rotate_binary_operator_on_binary_operator_if_needed( 
                binary_operator, binary_operator.left_operand.get<BinaryOperator>()
            );
        }
        if (binary_operator.left_operand.is<UnaryOperator>()) {
            return rotate_binary_operator_on_unary_operator_if_needed( 
                binary_operator, binary_operator.left_operand.get<UnaryOperator>()
            );
        }
    }
    return binary_operator;
}

[[nodiscard]] Expression Parser::rotate_binary_operator_on_binary_operator_if_needed(const BinaryOperator& parent, const BinaryOperator& left) {
    short parent_priority = infix_operators_priority.find(parent.operator_text)->second;
    short left_priority = infix_operators_priority.find(left.operator_text)->second;
    if (parent_priority <= left_priority) return parent;
    return BinaryOperator {
        left.as_token(), left.left_operand, rotate_binary_operator_to_match_operators_priority(
            BinaryOperator { parent.as_token(), left.right_operand, parent.right_operand }
        )
    };
}

[[nodiscard]] Expression Parser::rotate_binary_operator_on_unary_operator_if_needed(const BinaryOperator& parent, const UnaryOperator& left) {
    short parent_priority = infix_operators_priority.find(parent.operator_text)->second;
    short left_priority = prefix_operators_priority.find(left.operator_text)->second;
    if (parent_priority <= left_priority) return parent;
    return UnaryOperator {
        left.as_token(), rotate_binary_operator_to_match_operators_priority(
            BinaryOperator { parent.as_token(), left.operand, parent.right_operand }
        )
    };
}

[[nodiscard]] Expression Parser::rotate_to_match_is_operator_priority(const TypeOperator& type_operator) {
    if (type_operator.expression.is<BinaryOperator>()) {
        BinaryOperator rotated_expression = type_operator.expression.get<BinaryOperator>();
        rotated_expression.right_operand = rotate_to_match_is_operator_priority (
            TypeOperator { type_operator.as_token(), rotated_expression.right_operand, type_operator.typesignature }
        );
        return rotated_expression;
    }
    if (type_operator.expression.is<UnaryOperator>()) {
        UnaryOperator rotated_expression = type_operator.expression.get<UnaryOperator>();
        rotated_expression.operand = rotate_to_match_is_operator_priority (
            TypeOperator { type_operator.as_token(), rotated_expression.operand, type_operator.typesignature }
        );
        return rotated_expression;
    }
    return type_operator;
}