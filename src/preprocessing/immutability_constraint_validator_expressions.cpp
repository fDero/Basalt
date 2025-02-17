//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/immutability_constraint_validator.hpp"
using CCV = ImmutabilityConstraintValidator;

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_expression(const Expression& statement, ScopeContext& scope_context) {
    switch (statement.expression_kind()) {
        break; case ExpressionBody::Kind::function_call:         visit_function_call(statement.get<FunctionCall>(), scope_context);
        break; case ExpressionBody::Kind::type_operator:         visit_type_operator(statement.get<TypeOperator>(), scope_context);
        break; case ExpressionBody::Kind::binary_operator:       visit_binary_operator(statement.get<BinaryOperator>(), scope_context);
        break; case ExpressionBody::Kind::unary_operator:        visit_unary_operator(statement.get<UnaryOperator>(), scope_context);
        break; case ExpressionBody::Kind::array_literal:         visit_array_literal(statement.get<ArrayLiteral>(), scope_context);
        break; case ExpressionBody::Kind::dot_member_access:     visit_dot_member_access(statement.get<DotMemberAccess>(), scope_context);
        break; case ExpressionBody::Kind::square_bracket_access: visit_square_bracket_access(statement.get<SquareBracketsAccess>(), scope_context);
        break; case ExpressionBody::Kind::identifier:            return;
        break; case ExpressionBody::Kind::string_literal:        return;
        break; case ExpressionBody::Kind::float_literal:         return;
        break; case ExpressionBody::Kind::char_literal:          return;
        break; case ExpressionBody::Kind::int_literal:           return;
        break; case ExpressionBody::Kind::bool_literal:          return;
    }
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_type_operator(
    const TypeOperator& type_operator, 
    ScopeContext& scope_context
) {
    visit_expression(type_operator.expression, scope_context);
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_binary_operator(
    const BinaryOperator& binary_operator, 
    ScopeContext& scope_context
) {
    visit_expression(binary_operator.left_operand, scope_context);
    visit_expression(binary_operator.right_operand, scope_context);
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_unary_operator(
    const UnaryOperator& unary_operator, 
    ScopeContext& scope_context
) {
    visit_expression(unary_operator.operand, scope_context);
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_array_literal(
    const ArrayLiteral& array_literal, 
    ScopeContext& scope_context
) {
    for (const Expression& element : array_literal.elements) {
        visit_expression(element, scope_context);
    }
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_dot_member_access(
    const DotMemberAccess& dot_member_access, 
    ScopeContext& scope_context
) {
    visit_expression(dot_member_access.struct_value, scope_context);
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_square_bracket_access(
    const SquareBracketsAccess& square_brackets_access, 
    ScopeContext& scope_context
) {
    visit_expression(square_brackets_access.storage, scope_context);
}