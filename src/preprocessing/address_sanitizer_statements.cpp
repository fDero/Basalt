//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/address_sanitizer.hpp"
using AS = AddressSanitizer;

void AS::SingleFunctionAddressSanitizer::visit_statement(const Statement& statement) {
    switch (statement.statement_kind()) {
        break; case StatementBody::Kind::function_call:        visit_function_call(statement.get<FunctionCall>());
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

void AS::SingleFunctionAddressSanitizer::visit_variable_declaration(const VariableDeclaration& variable_declaration) {
    if (variable_declaration.initial_value.has_value()) {
        visit_expression(variable_declaration.initial_value.value());
    }
}

void AS::SingleFunctionAddressSanitizer::visit_const_declaration(const ConstDeclaration& const_declaration) {
    visit_expression(const_declaration.value);
}

void AS::SingleFunctionAddressSanitizer::visit_assignment(const Assignment& assignment) {
    visit_expression(assignment.assignment_target);
    visit_expression(assignment.assigned_value);
}

void AS::SingleFunctionAddressSanitizer::visit_conditional(const Conditional& conditional) {
    visit_expression(conditional.condition);
    visit_code_block(conditional.then_branch);
    visit_code_block(conditional.else_branch);
}

void AS::SingleFunctionAddressSanitizer::visit_while_loop(const WhileLoop& loop) {
    visit_expression(loop.condition);
    visit_code_block(loop.loop_body);
}

void AS::SingleFunctionAddressSanitizer::visit_until_loop(const UntilLoop& loop) {
    visit_expression(loop.condition);
    visit_code_block(loop.loop_body);
}

void AS::SingleFunctionAddressSanitizer::visit_return_statement(const Return& ret) {
    if (ret.return_value.has_value()) {
        visit_expression(ret.return_value.value());
    }
}