//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/generics_substitution_rules.hpp"
#include "core/generics_instantiation_engine.hpp"
#include "errors/internal_errors.hpp"

Expression GenericsInstantiationEngine::instantiate_generic_expression(const Expression& expression) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::function_call:         return instantiate_generic_function_call(expression.get<FunctionCall>());
        case ExpressionBody::Kind::binary_operator:       return instantiate_generic_binary_operator(expression.get<BinaryOperator>());
        case ExpressionBody::Kind::unary_operator:        return instantiate_generic_unary_operator(expression.get<UnaryOperator>());
        case ExpressionBody::Kind::array_literal:         return instantiate_generic_array_literal(expression.get<ArrayLiteral>());
        case ExpressionBody::Kind::type_operator:         return instantiate_generic_type_operator(expression.get<TypeOperator>());
        case ExpressionBody::Kind::dot_member_access:     return instantiate_generic_dot_member_access(expression.get<DotMemberAccess>());
        case ExpressionBody::Kind::square_bracket_access: return instantiate_generic_square_bracket_access(expression.get<SquareBracketsAccess>());
        case ExpressionBody::Kind::int_literal:           return expression;
        case ExpressionBody::Kind::float_literal:         return expression;
        case ExpressionBody::Kind::char_literal:          return expression;
        case ExpressionBody::Kind::bool_literal:          return expression;
        case ExpressionBody::Kind::string_literal:        return expression;
        case ExpressionBody::Kind::identifier:            return expression;
    }
    assert_unreachable();
}

FunctionCall GenericsInstantiationEngine::instantiate_generic_function_call(const FunctionCall& expression) {
    FunctionCall instantiated_function_call = expression;
    for (Expression& arg : instantiated_function_call.arguments) {
        arg = instantiate_generic_expression(arg);
    }
    for (TypeSignature& type : instantiated_function_call.instantiated_generics) {
        type = instantiate_generic_typesignature(type);
    }
    return instantiated_function_call;
}

BinaryOperator GenericsInstantiationEngine::instantiate_generic_binary_operator(const BinaryOperator& expression) {
    BinaryOperator instantiated_binary_operator = expression;
    instantiated_binary_operator.left_operand = instantiate_generic_expression(expression.left_operand);
    instantiated_binary_operator.right_operand = instantiate_generic_expression(expression.right_operand);
    return instantiated_binary_operator;
}

UnaryOperator GenericsInstantiationEngine::instantiate_generic_unary_operator(const UnaryOperator& expression) {
    UnaryOperator instantiated_unary_operator = expression;
    instantiated_unary_operator.operand = instantiate_generic_expression(expression.operand);
    return instantiated_unary_operator;
}

ArrayLiteral GenericsInstantiationEngine::instantiate_generic_array_literal(const ArrayLiteral& expression) {
    ArrayLiteral instantiated_array_literal = expression;
    instantiated_array_literal.array_length = expression.array_length;
    instantiated_array_literal.stored_type = instantiate_generic_typesignature(expression.stored_type);
    for (Expression& element : instantiated_array_literal.elements) {
        element = instantiate_generic_expression(element);
    }
    return instantiated_array_literal;
}

TypeOperator GenericsInstantiationEngine::instantiate_generic_type_operator(const TypeOperator& expression) {
    TypeOperator instantiated_type_operator = expression;
    instantiated_type_operator.expression = instantiate_generic_expression(expression.expression);
    instantiated_type_operator.typesignature = instantiate_generic_typesignature(expression.typesignature);
    return instantiated_type_operator;
}

DotMemberAccess GenericsInstantiationEngine::instantiate_generic_dot_member_access(const DotMemberAccess& expression) {
    DotMemberAccess instantiated_dot_member_access = expression;
    instantiated_dot_member_access.struct_value = instantiate_generic_expression(expression.struct_value);
    return instantiated_dot_member_access;
}

SquareBracketsAccess GenericsInstantiationEngine::instantiate_generic_square_bracket_access(const SquareBracketsAccess& expression) {
    SquareBracketsAccess instantiated_square_brackets_access = expression;
    instantiated_square_brackets_access.storage = instantiate_generic_expression(expression.storage);
    instantiated_square_brackets_access.index = instantiate_generic_expression(expression.index);
    return instantiated_square_brackets_access;
}