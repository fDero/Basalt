//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/immutability_deducer.hpp"
#include "errors/internal_errors.hpp"
#include "syntax/infixes.hpp"

ImmutabilityDeducer::ImmutabilityDeducer(
    ScopeContext& scope_context, 
    ProgramRepresentation& program_representation
)
    : scope_context(scope_context)
    , program_representation(program_representation) 
{}

bool ImmutabilityDeducer::is_expression_immutable(const Expression& expression) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::identifier:            return is_identifier_immutable(expression.get<Identifier>());
        case ExpressionBody::Kind::type_operator:         return is_type_operator_immutable(expression.get<TypeOperator>());
        case ExpressionBody::Kind::dot_member_access:     return is_dot_member_access_immutable(expression.get<DotMemberAccess>());
        case ExpressionBody::Kind::square_bracket_access: return is_square_bracket_access_immutable(expression.get<SquareBracketsAccess>());
        case ExpressionBody::Kind::unary_operator:        return is_unary_operator_immutable(expression.get<UnaryOperator>());
        case ExpressionBody::Kind::function_call:         return is_function_call_immutable(expression.get<FunctionCall>());
        case ExpressionBody::Kind::binary_operator:       return true;
        case ExpressionBody::Kind::array_literal:         return true;
        case ExpressionBody::Kind::string_literal:        return true;
        case ExpressionBody::Kind::float_literal:         return true;
        case ExpressionBody::Kind::char_literal:          return true;
        case ExpressionBody::Kind::int_literal:           return true;
        case ExpressionBody::Kind::bool_literal:          return true;
    }
    assert_unreachable();
}

bool ImmutabilityDeducer::is_identifier_immutable(const Identifier& identifier) {
    return scope_context.is_identifier_immutable(identifier.name);
}

bool ImmutabilityDeducer::is_square_bracket_access_immutable(const SquareBracketsAccess& square_brackets_access) {
    return is_expression_immutable(square_brackets_access.storage);
}

bool ImmutabilityDeducer::is_dot_member_access_immutable(const DotMemberAccess& dot_member_access) {
    return is_expression_immutable(dot_member_access.struct_value);
}

bool ImmutabilityDeducer::is_type_operator_immutable(const TypeOperator& type_operator) {
    return type_operator.operator_text == as_keyword && is_expression_immutable(type_operator.expression);
}

bool ImmutabilityDeducer::is_unary_operator_immutable(const UnaryOperator& unary_operator) {
    return (unary_operator.operator_text != "#" && unary_operator.operator_text != "&") ||
        is_expression_immutable(unary_operator.operand);
}

bool ImmutabilityDeducer::is_function_call_immutable(const FunctionCall& function_call) {
    return false;
}