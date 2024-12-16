//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/assignability_checker.hpp"

AssignabilityChecker::AssignabilityChecker(
    ScopeContext& scope_context, 
    ProgramRepresentation& program_representation,
    BondInspector& bond_inspector,
    ImmutabilityChecker& immutability_checker
) 
    : scope_context(scope_context)
    , program_representation(program_representation) 
    , bond_inspector(bond_inspector)   
    , immutability_checker(immutability_checker) 
{}
        
bool AssignabilityChecker::is_expression_assignable_to_var(const Expression& expression) {
    switch (expression.expression_kind()) {
        case ExpressionBody::Kind::identifier:            return is_potentially_bonding_expression_assignable_to_var(expression);
        case ExpressionBody::Kind::dot_member_access:     return is_potentially_bonding_expression_assignable_to_var(expression);
        case ExpressionBody::Kind::square_bracket_access: return is_potentially_bonding_expression_assignable_to_var(expression);
        case ExpressionBody::Kind::unary_operator:        return is_potentially_bonding_expression_assignable_to_var(expression);
        case ExpressionBody::Kind::array_literal:         return is_potentially_bonding_expression_assignable_to_var(expression);
        case ExpressionBody::Kind::type_operator:         return is_potentially_bonding_expression_assignable_to_var(expression);
        case ExpressionBody::Kind::function_call:         return true;
        case ExpressionBody::Kind::binary_operator:       return true;
        case ExpressionBody::Kind::string_literal:        return true;
        case ExpressionBody::Kind::float_literal:         return true;
        case ExpressionBody::Kind::char_literal:          return true;
        case ExpressionBody::Kind::int_literal:           return true;
        case ExpressionBody::Kind::bool_literal:          return true;
    }
    assert_unreachable();
}

bool AssignabilityChecker::is_potentially_bonding_expression_assignable_to_var(const Expression& expression) {
    auto expr_type = program_representation.resolve_expression_type(expression, scope_context);
    bool assignment_of_immutable_expr = immutability_checker.is_weakly_immutable_expression(expression);
    bool assignment_implies_bond = expr_type.has_value() && bond_inspector.does_the_type_of_this_expr_imply_a_bond(*expr_type);
    return !assignment_of_immutable_expr || !assignment_implies_bond;
}