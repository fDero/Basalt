
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] Expression Parser::rotate_binary_operator_to_match_operators_priority(const BinaryOperator& binary_operator){
    if (!binary_operator.left_operand.is_wrapped_in_in_parenthesis()) {
        if (binary_operator.left_operand.is<BinaryOperator>()){
            return rotate_binary_operator_on_binary_operator_if_needed( 
                binary_operator, binary_operator.left_operand.get<BinaryOperator>()
            );
        }
        if (binary_operator.left_operand.is<UnaryOperator>()){
            return rotate_binary_operator_on_unary_operator_if_needed( 
                binary_operator, binary_operator.left_operand.get<UnaryOperator>()
            );
        }
    }
    return binary_operator;
}

[[nodiscard]] Expression Parser::rotate_binary_operator_on_binary_operator_if_needed(const BinaryOperator& parent, const BinaryOperator& left){
    short parent_priority = infix_operators_priority.find(parent.operator_text)->second;
    short left_priority = infix_operators_priority.find(left.operator_text)->second;
    if (parent_priority <= left_priority) return parent;
    return BinaryOperator {
        left.operator_text, left.left_operand, rotate_binary_operator_to_match_operators_priority(
            BinaryOperator { parent.operator_text, left.right_operand, parent.right_operand }
        )
    };
}

[[nodiscard]] Expression Parser::rotate_binary_operator_on_unary_operator_if_needed(const BinaryOperator& parent, const UnaryOperator& left){
    short parent_priority = infix_operators_priority.find(parent.operator_text)->second;
    short left_priority = prefix_operators_priority.find(left.operator_text)->second;
    if (parent_priority <= left_priority) return parent;
    return UnaryOperator {
        left.operator_text, rotate_binary_operator_to_match_operators_priority(
            BinaryOperator { parent.operator_text, left.operand, parent.right_operand }
        )
    };
}