//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/syntax.hpp"
#include "frontend/parser.hpp"
#include "language/expressions.hpp"

Expression Parser::rotate_binary_operator_to_match_operators_priority(const BinaryOperator& binary_operator) {
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

Expression Parser::rotate_binary_operator_on_binary_operator_if_needed(const BinaryOperator& parent, const BinaryOperator& left) {
    short parent_priority = infix_operators_priority.find(parent.operator_text)->second;
    short left_priority = infix_operators_priority.find(left.operator_text)->second;
    if (parent_priority <= left_priority) return parent;
    return BinaryOperator {
        left.as_token(), left.left_operand, rotate_binary_operator_to_match_operators_priority(
            BinaryOperator { parent.as_token(), left.right_operand, parent.right_operand }
        )
    };
}

Expression Parser::rotate_binary_operator_on_unary_operator_if_needed(const BinaryOperator& parent, const UnaryOperator& left) {
    short parent_priority = infix_operators_priority.find(parent.operator_text)->second;
    short left_priority = prefix_operators_priority.find(left.operator_text)->second;
    if (parent_priority <= left_priority) return parent;
    return UnaryOperator {
        left.as_token(), rotate_binary_operator_to_match_operators_priority(
            BinaryOperator { parent.as_token(), left.operand, parent.right_operand }
        )
    };
}

Expression Parser::rotate_to_match_type_operator_priority(const TypeOperator& type_operator) {
    if (type_operator.expression.is<BinaryOperator>()) {
        BinaryOperator rotated_expression = type_operator.expression.get<BinaryOperator>();
        rotated_expression.right_operand = rotate_to_match_type_operator_priority (
            TypeOperator { type_operator.as_token(), rotated_expression.right_operand, type_operator.typesignature }
        );
        return rotated_expression;
    }
    if (type_operator.expression.is<UnaryOperator>()) {
        UnaryOperator rotated_expression = type_operator.expression.get<UnaryOperator>();
        rotated_expression.operand = rotate_to_match_type_operator_priority (
            TypeOperator { type_operator.as_token(), rotated_expression.operand, type_operator.typesignature }
        );
        return rotated_expression;
    }
    return type_operator;
}

Expression Parser::rotate_to_match_dot_member_access_priority(const DotMemberAccess& dot_member_access) {
    const DebugInformationsAwareEntity& debug_info = dot_member_access.as_debug_informations_aware_entity();
    if (dot_member_access.struct_value.is<BinaryOperator>()) {
        BinaryOperator rotated_expression = dot_member_access.struct_value.get<BinaryOperator>();
        rotated_expression.right_operand = rotate_to_match_dot_member_access_priority (
            DotMemberAccess { debug_info, rotated_expression.right_operand, dot_member_access.member_name }
        );
        return rotated_expression;
    }
    if (dot_member_access.struct_value.is<UnaryOperator>()) {
        UnaryOperator rotated_expression = dot_member_access.struct_value.get<UnaryOperator>();
        rotated_expression.operand = rotate_to_match_dot_member_access_priority (
            DotMemberAccess { debug_info, rotated_expression.operand, dot_member_access.member_name }
        );
        return rotated_expression;
    }
    return dot_member_access;
}

Expression Parser::rotate_to_match_square_brackets_access_priority(const SquareBracketsAccess& square_brackets_access) {
    const DebugInformationsAwareEntity& debug_info = square_brackets_access.as_debug_informations_aware_entity();
    if (square_brackets_access.storage.is<BinaryOperator>()) {
        BinaryOperator rotated_expression = square_brackets_access.storage.get<BinaryOperator>();
        rotated_expression.right_operand = rotate_to_match_square_brackets_access_priority (
            SquareBracketsAccess { debug_info, rotated_expression.right_operand, square_brackets_access.index }
        );
        return rotated_expression;
    }
    if (square_brackets_access.storage.is<UnaryOperator>()) {
        UnaryOperator rotated_expression = square_brackets_access.storage.get<UnaryOperator>();
        rotated_expression.operand = rotate_to_match_square_brackets_access_priority (
            SquareBracketsAccess { debug_info, rotated_expression.operand, square_brackets_access.index }
        );
        return rotated_expression;
    }
    return square_brackets_access;
}