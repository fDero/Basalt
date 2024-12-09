//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/immutability_checker.hpp"
#include "errors/internal_errors.hpp"

ImmutabilityChecker::ImmutabilityChecker(
    ScopeContext& scope_context, 
    ProgramRepresentation& program_representation
)
    : scope_context(scope_context)
    , program_representation(program_representation) 
{}

bool ImmutabilityChecker::is_weakly_immutable_expression(const Expression& expression) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::function_call: 
            return is_function_call_weakly_immutable(expression.get<FunctionCall>());
        case ExpressionBody::Kind::unary_operator: 
            return is_unary_weakly_operator_immutable(expression.get<UnaryOperator>());
        default: 
            return is_strictly_immutable_expression(expression);
    }
}

bool ImmutabilityChecker::is_strictly_immutable_expression(const Expression& expression) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::identifier:            return is_identifier_immutable(expression.get<Identifier>());
        case ExpressionBody::Kind::type_operator:         return is_type_operator_immutable(expression.get<TypeOperator>());
        case ExpressionBody::Kind::dot_member_access:     return is_dot_member_access_immutable(expression.get<DotMemberAccess>());
        case ExpressionBody::Kind::square_bracket_access: return is_square_bracket_access_immutable(expression.get<SquareBracketsAccess>());
        case ExpressionBody::Kind::unary_operator:        return is_unary_operator_immutable(expression.get<UnaryOperator>());
        case ExpressionBody::Kind::function_call:         return true;
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

bool ImmutabilityChecker::is_identifier_immutable(const Identifier& identifier) {
    return scope_context.is_identifier_immutable(identifier.name);
}

bool ImmutabilityChecker::is_unary_operator_immutable(const UnaryOperator& unary_operator) {
    return unary_operator.operator_text != "#" || 
        is_weakly_immutable_expression(unary_operator.operand);
}

bool ImmutabilityChecker::is_square_bracket_access_immutable(const SquareBracketsAccess& square_brackets_access) {
    return is_weakly_immutable_expression(square_brackets_access.storage);
}

bool ImmutabilityChecker::is_dot_member_access_immutable(const DotMemberAccess& dot_member_access) {
    return is_weakly_immutable_expression(dot_member_access.struct_value);
}

bool ImmutabilityChecker::is_type_operator_immutable(const TypeOperator& type_operator) {
    return type_operator.operator_text == "as" && is_weakly_immutable_expression(type_operator.expression);
}

bool ImmutabilityChecker::is_function_call_weakly_immutable(const FunctionCall& function_call) {
    std::optional<TypeSignature> return_type = program_representation.resolve_expression_type(function_call, scope_context);
    return !return_type.has_value() || (!return_type->is<PointerType>() && !return_type->is<SliceType>());
}

bool ImmutabilityChecker::is_unary_weakly_operator_immutable(const UnaryOperator& unary_operator) {
    return (unary_operator.operator_text != "#" && unary_operator.operator_text != "&") ||
        is_weakly_immutable_expression(unary_operator.operand);
}