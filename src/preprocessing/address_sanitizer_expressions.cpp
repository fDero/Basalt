//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/preprocessing_errors.hpp"
#include "preprocessing/address_sanitizer.hpp"
using AS = AddressSanitizer;

void AS::SingleFunctionAddressSanitizer::visit_expression(const Expression& expr) {
    switch (expr.expression_kind()) {
        break; case ExpressionBody::Kind::function_call:   visit_function_call(expr.get<FunctionCall>());
        break; case ExpressionBody::Kind::type_operator:   visit_type_operator(expr.get<TypeOperator>());    
        break; case ExpressionBody::Kind::binary_operator: visit_binary_operator(expr.get<BinaryOperator>());
        break; case ExpressionBody::Kind::unary_operator:  visit_unary_operator(expr.get<UnaryOperator>());
        break; case ExpressionBody::Kind::array_literal:   visit_array_literal(expr.get<ArrayLiteral>());
        break; case ExpressionBody::Kind::dot_member_access:
            visit_dot_member_access(expr.get<DotMemberAccess>());
        break; case ExpressionBody::Kind::square_bracket_access:
            visit_square_bracket_access(expr.get<SquareBracketsAccess>());
        break; case ExpressionBody::Kind::identifier:      return;
        break; case ExpressionBody::Kind::string_literal:  return;
        break; case ExpressionBody::Kind::float_literal:   return;
        break; case ExpressionBody::Kind::char_literal:    return;
        break; case ExpressionBody::Kind::int_literal:     return;
        break; case ExpressionBody::Kind::bool_literal:    return;
    }
}

bool AS::SingleFunctionAddressSanitizer::is_expression_addressable(const Expression& expr) {
    switch (expr.expression_kind()) {
        case ExpressionBody::Kind::dot_member_access:     return true;
        case ExpressionBody::Kind::square_bracket_access: return true;
        case ExpressionBody::Kind::identifier:            return true;
        case ExpressionBody::Kind::function_call:         return false;
        case ExpressionBody::Kind::binary_operator:       return false;
        case ExpressionBody::Kind::array_literal:         return false;
        case ExpressionBody::Kind::string_literal:        return false;
        case ExpressionBody::Kind::float_literal:         return false;
        case ExpressionBody::Kind::char_literal:          return false;
        case ExpressionBody::Kind::int_literal:           return false;
        case ExpressionBody::Kind::bool_literal:          return false;
        case ExpressionBody::Kind::type_operator:
            return is_expression_addressable(expr.get<TypeOperator>().expression);    
        case ExpressionBody::Kind::unary_operator:
            return expr.get<UnaryOperator>().operator_text == pointer_dereference_operator;
    }
    return false;
}

void AS::SingleFunctionAddressSanitizer::visit_type_operator(const TypeOperator& type_operator) {
    visit_expression(type_operator.expression);
}

void AS::SingleFunctionAddressSanitizer::visit_binary_operator(const BinaryOperator& binary_operator) {
    visit_expression(binary_operator.left_operand);
    visit_expression(binary_operator.right_operand);
}

void AS::SingleFunctionAddressSanitizer::visit_unary_operator(const UnaryOperator& unary_operator) {
    if (unary_operator.operator_text == address_operator) {
        bool is_addressable = is_expression_addressable(unary_operator.operand);
        ensure_not_trying_to_dereference_a_literal(unary_operator, is_addressable);
    }
    visit_expression(unary_operator.operand);
}

void AS::SingleFunctionAddressSanitizer::visit_array_literal(const ArrayLiteral& array_literal) {
    for (const Expression& element : array_literal.elements) {
        visit_expression(element);
    }
}

void AS::SingleFunctionAddressSanitizer::visit_dot_member_access(const DotMemberAccess& dot_member_access) {
    visit_expression(dot_member_access.struct_value);
}

void AS::SingleFunctionAddressSanitizer::visit_square_bracket_access(const SquareBracketsAccess& square_brackets_access) {
    visit_expression(square_brackets_access.storage);
    visit_expression(square_brackets_access.index);
}