//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/function_definitions_typechecker.hpp"
using FDTC = FunctionDefinitionsTypeChecker;

void FDTC::SingleFunctionTypeChecker::visit_statement(const Statement& statement) {
    switch (statement.statement_kind()) {
        break; case StatementBody::Kind::function_call:        visit_function_call(statement.get<FunctionCall>(), false);
        break; case StatementBody::Kind::variable_declaration: visit_variable_declaration(statement.get<VariableDeclaration>());
        break; case StatementBody::Kind::const_declaration:    visit_const_declaration(statement.get<ConstDeclaration>());
        break; case StatementBody::Kind::assignment:           visit_assignment(statement.get<Assignment>());
        break; case StatementBody::Kind::conditional:          visit_conditional(statement.get<Conditional>());
        break; case StatementBody::Kind::while_loop:           visit_while_loop(statement.get<WhileLoop>());
        break; case StatementBody::Kind::until_loop:           visit_until_loop(statement.get<UntilLoop>());
        break; case StatementBody::Kind::return_statement:     visit_return_statement(statement.get<Return>());
        break; case StatementBody::Kind::break_statement:      return;
        break; case StatementBody::Kind::continue_statement:   return;
    }
}

void FDTC::SingleFunctionTypeChecker::visit_variable_declaration(const VariableDeclaration& variable_declaration) {
    program_representation.verify_that_the_type_exists(variable_declaration.typesignature);
    if (variable_declaration.initial_value.has_value()) {
        visit_expression(variable_declaration.initial_value.value());
        auto expr_type = program_representation.resolve_expression_type(variable_declaration.initial_value.value(), scope_context);
        bool assignment_is_valid = program_representation.validate_assignment(expr_type, variable_declaration.typesignature);
        ensure_variable_declaration_is_valid(assignment_is_valid, variable_declaration, expr_type);
    }
    scope_context.store_local_variable(variable_declaration);
}

void FDTC::SingleFunctionTypeChecker::visit_const_declaration(const ConstDeclaration& const_declaration) {
    program_representation.verify_that_the_type_exists(const_declaration.typesignature);
    visit_expression(const_declaration.value);
    auto expr_type = program_representation.resolve_expression_type(const_declaration.value, scope_context);
    bool assignment_is_valid = program_representation.validate_assignment(expr_type, const_declaration.typesignature);
    ensure_const_declaration_is_valid(assignment_is_valid, const_declaration, expr_type);
    scope_context.store_local_constant(const_declaration);
}

void FDTC::SingleFunctionTypeChecker::visit_assignment(const Assignment& assignment) {
    visit_expression(assignment.assignment_target);
    visit_expression(assignment.assigned_value);
    auto target_type = program_representation.resolve_expression_type(assignment.assignment_target, scope_context);
    auto assigned_value_type = program_representation.resolve_expression_type(assignment.assigned_value, scope_context);
    bool assignment_is_valid = program_representation.validate_assignment(assigned_value_type, target_type);
    ensure_assignment_is_valid(assignment_is_valid, assignment);
}

void FDTC::SingleFunctionTypeChecker::visit_conditional(const Conditional& conditional) {
    visit_expression(conditional.condition);
    auto condition_type = program_representation.resolve_expression_type(conditional.condition, scope_context);
    ensure_condition_expression_is_boolean(condition_type, conditional.condition);
    visit_code_block(conditional.then_branch);
    visit_code_block(conditional.else_branch);
}

void FDTC::SingleFunctionTypeChecker::visit_while_loop(const WhileLoop& loop) {
    visit_expression(loop.condition);
    auto condition_type = program_representation.resolve_expression_type(loop.condition, scope_context);
    ensure_condition_expression_is_boolean(condition_type, loop.condition);
    visit_code_block(loop.loop_body);
}

void FDTC::SingleFunctionTypeChecker::visit_until_loop(const UntilLoop& loop) {
    visit_expression(loop.condition);
    auto condition_type = program_representation.resolve_expression_type(loop.condition, scope_context);
    ensure_condition_expression_is_boolean(condition_type, loop.condition);
    visit_code_block(loop.loop_body);
}

void FDTC::SingleFunctionTypeChecker::visit_return_statement(const Return& ret) {
    bool return_value_is_provided = ret.return_value.has_value();
    bool return_value_is_expected = expected_return_type.has_value();
    ensure_return_statement_is_congruent_to_function_definition(return_value_is_expected, return_value_is_provided);
    if (return_value_is_provided) {
        visit_expression(ret.return_value.value());
        auto return_value_type = program_representation
            .resolve_expression_type(ret.return_value.value(), scope_context);
        bool valid_assignment = program_representation.validate_assignment(return_value_type, expected_return_type);
        ensure_return_statement_is_valid(valid_assignment, ret);
    }
}