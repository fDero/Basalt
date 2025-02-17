//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/assignment_immutability_checker.hpp"

AssignmentImmutabilityChecker::AssignmentImmutabilityChecker(
    ScopeContext& scope_context, 
    ProgramRepresentation& program_representation
) 
    : scope_context(scope_context)
    , program_representation(program_representation) 
    , bond_inspector(scope_context, program_representation)   
    , immutability_deducer(scope_context, program_representation) 
{}
        
bool AssignmentImmutabilityChecker::is_expression_assignable_to_var(const Expression& expression) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::identifier:            
        case ExpressionBody::Kind::dot_member_access:     
        case ExpressionBody::Kind::square_bracket_access: 
        case ExpressionBody::Kind::unary_operator:        
        case ExpressionBody::Kind::array_literal:         
        case ExpressionBody::Kind::type_operator:         
            return is_potentially_bonding_expression_assignable_to_var(expression);
        
        case ExpressionBody::Kind::function_call:  
        case ExpressionBody::Kind::binary_operator:
        case ExpressionBody::Kind::string_literal: 
        case ExpressionBody::Kind::float_literal:  
        case ExpressionBody::Kind::char_literal:   
        case ExpressionBody::Kind::int_literal:    
        case ExpressionBody::Kind::bool_literal:          
            return true;
    }
    assert_unreachable();
}

bool AssignmentImmutabilityChecker::is_potentially_bonding_expression_assignable_to_var(const Expression& expression) {
    auto expr_type = program_representation.resolve_expression_type(expression, scope_context);
    bool assignment_of_immutable_expr = immutability_deducer.is_expression_immutable(expression);
    bool assignment_implies_bond = expr_type.has_value() && bond_inspector.does_the_type_of_this_expr_imply_a_bond(*expr_type);
    return !assignment_of_immutable_expr || !assignment_implies_bond;
}

bool AssignmentImmutabilityChecker::does_assignment_discard_qualifiers(const Expression& source, const Expression& dest) {
    bool assignment_to_immutable_target = immutability_deducer.is_expression_immutable(dest);
    bool assignment_to_final_target = false;
    if (dest.is<UnaryOperator>()) {
        auto unary_operator = dest.get<UnaryOperator>();
        assignment_to_final_target = unary_operator.operator_text == "&";
    }
    bool expression_not_assignable = !is_expression_assignable_to_var(source);
    return assignment_to_immutable_target || assignment_to_final_target || expression_not_assignable;
}

bool AssignmentImmutabilityChecker::does_assignment_discard_qualifiers(const Assignment& assignment) {
    return does_assignment_discard_qualifiers(
        assignment.assigned_value, 
        assignment.assignment_target
    );
}