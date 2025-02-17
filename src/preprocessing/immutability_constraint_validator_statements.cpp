//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/assignment_immutability_checker.hpp"
#include "preprocessing/immutability_constraint_validator.hpp"
using CCV = ImmutabilityConstraintValidator;

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_statement(
    const Statement& statement, 
    ScopeContext& scope_context
) {
    switch (statement.statement_kind()) {
        break; case StatementBody::Kind::return_statement: visit_return_statement(statement.get<Return>(), scope_context);
        break; case StatementBody::Kind::conditional: visit_if_statement(statement.get<Conditional>(), scope_context);
        break; case StatementBody::Kind::while_loop: visit_while_loop(statement.get<WhileLoop>(), scope_context);
        break; case StatementBody::Kind::until_loop: visit_until_loop(statement.get<UntilLoop>(), scope_context);
        break; case StatementBody::Kind::function_call: visit_function_call(statement.get<FunctionCall>(), scope_context);
        break; case StatementBody::Kind::variable_declaration: visit_variable_declaration(statement.get<VariableDeclaration>(), scope_context);
        break; case StatementBody::Kind::const_declaration: visit_const_declaration(statement.get<ConstDeclaration>(), scope_context);
        break; case StatementBody::Kind::assignment: visit_assignment(statement.get<Assignment>(), scope_context);
        break; case StatementBody::Kind::break_statement: return;
        break; case StatementBody::Kind::continue_statement: return;
    }
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_if_statement(
    const Conditional& conditional, 
    ScopeContext& scope_context
) {
    visit_expression(conditional.condition, scope_context);
    ScopeContext then_scope_context = scope_context.create_nested_scope();
    ScopeContext else_scope_context = scope_context.create_nested_scope();
    for (const Statement& statement : conditional.then_branch) {
        visit_statement(statement, then_scope_context);
    }
    for (const Statement& statement : conditional.else_branch) {
        visit_statement(statement, else_scope_context);
    }
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_while_loop(
    const WhileLoop& while_loop, 
    ScopeContext& scope_context
) {
    visit_expression(while_loop.condition, scope_context);
    ScopeContext loop_scope_context = scope_context.create_nested_scope();
    for (const Statement& statement : while_loop.loop_body) {
        visit_statement(statement, loop_scope_context);
    }
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_until_loop(
    const UntilLoop& until_loop, 
    ScopeContext& scope_context
) {
    visit_expression(until_loop.condition, scope_context);
    ScopeContext loop_scope_context = scope_context.create_nested_scope();
    for (const Statement& statement : until_loop.loop_body) {
        visit_statement(statement, loop_scope_context);
    }
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_return_statement(
    const Return& return_statement, 
    ScopeContext& scope_context
) {
    if (return_statement.return_value.has_value()) {
        visit_expression(return_statement.return_value.value(), scope_context);
    }
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_variable_declaration(
    const VariableDeclaration& variable_declaration, 
    ScopeContext& scope_context
) {
    if (variable_declaration.initial_value.has_value()) {
        Expression value = variable_declaration.initial_value.value();
        visit_expression(variable_declaration.initial_value.value(), scope_context);
        AssignmentImmutabilityChecker assignment_immutability_checker(scope_context, program_representation);
        bool assignment_discard_qualifiers = !assignment_immutability_checker.is_expression_assignable_to_var(value);
        ensure_assignment_complies_with_const_qualifiers(variable_declaration, assignment_discard_qualifiers);
    }
    scope_context.store_local_variable(variable_declaration);
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_const_declaration(
    const ConstDeclaration& const_declaration, 
    ScopeContext& scope_context
) {
    visit_expression(const_declaration.value, scope_context);
    scope_context.store_local_constant(const_declaration);
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_function_call(
    const FunctionCall& function_call, 
    ScopeContext& scope_context
) {
    for (const Expression& argument : function_call.arguments) {
        visit_expression(argument, scope_context);
        AssignmentImmutabilityChecker assignment_immutability_checker(scope_context, program_representation);
        bool assignment_discard_qualifiers = !assignment_immutability_checker.is_expression_assignable_to_var(argument);
        ensure_use_as_function_argument_complies_with_const_qualifiers(function_call, argument, assignment_discard_qualifiers);
    }
}

void CCV::SingleFunctionImmutabilityConstraintValidator::visit_assignment(
    const Assignment& assignment, 
    ScopeContext& scope_context
) {
    visit_expression(assignment.assigned_value, scope_context);
    AssignmentImmutabilityChecker assignment_immutability_checker(scope_context, program_representation);
    bool assignment_discards_qualifiers = assignment_immutability_checker.does_assignment_discard_qualifiers(assignment);
    ensure_assignment_complies_with_const_qualifiers(assignment, assignment_discards_qualifiers);
}