//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/observability_deducer.hpp"
#include "errors/internal_errors.hpp"
#include "syntax/infixes.hpp"

ObservabilityDeducer::ObservabilityDeducer(
    ScopeContext& scope_context, 
    ProgramRepresentation& program_representation
) 
    : scope_context(scope_context)
    , program_representation(program_representation) 
{}

bool ObservabilityDeducer::is_expression_update_observable(const Expression& expression) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::dot_member_access:     return is_dot_member_access_update_observable(expression.get<DotMemberAccess>());
        case ExpressionBody::Kind::square_bracket_access: return is_square_bracket_access_update_observable(expression.get<SquareBracketsAccess>());
        case ExpressionBody::Kind::unary_operator:        return is_unary_operator_update_observable(expression.get<UnaryOperator>());
        case ExpressionBody::Kind::function_call:         return is_function_call_update_observable(expression.get<FunctionCall>());
        case ExpressionBody::Kind::type_operator:         return is_type_operator_update_observable(expression.get<TypeOperator>());
        case ExpressionBody::Kind::identifier:            return true;
 
        case ExpressionBody::Kind::binary_operator: 
        case ExpressionBody::Kind::array_literal:   
        case ExpressionBody::Kind::string_literal:  
        case ExpressionBody::Kind::float_literal:   
        case ExpressionBody::Kind::char_literal:    
        case ExpressionBody::Kind::int_literal:     
        case ExpressionBody::Kind::bool_literal:    
            return false;
    }
    assert_unreachable();
}

bool ObservabilityDeducer::is_unary_operator_update_observable(const UnaryOperator& unary_opertor) {
    return unary_opertor.operator_text == "#" || unary_opertor.operator_text == "&";
}

bool ObservabilityDeducer::is_square_bracket_access_update_observable(const SquareBracketsAccess& square_brackets_access) {
    return is_expression_update_observable(square_brackets_access.storage);
}

bool ObservabilityDeducer::is_dot_member_access_update_observable(const DotMemberAccess& dot_member_access) {
    return is_expression_update_observable(dot_member_access.struct_value);
}

bool ObservabilityDeducer::is_type_operator_update_observable(const TypeOperator& type_operator) {
    return type_operator.operator_text != as_keyword && is_expression_update_observable(type_operator.expression);
}

bool ObservabilityDeducer::is_function_call_update_observable(const FunctionCall& function_call) {
    auto return_type_opt = program_representation.resolve_expression_type(function_call, scope_context);
    return return_type_opt.has_value() && (return_type_opt.value().is<PointerType>() || return_type_opt.value().is<SliceType>());
}