//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/function_exit_path_navigator.hpp"

FunctionExitPathNavigator::FunctionExitPathNavigator(
    ProgramRepresentation& program_representation
)
    : program_representation(program_representation) 
{}

void FunctionExitPathNavigator::visit_all_function_definitions() {
    program_representation.foreach_function_definition([&](const FunctionDefinition::Ref& function_definition) {
        visit_function_definition(*function_definition);
    });
}

void FunctionExitPathNavigator::visit_function_definition(
    const FunctionDefinition& function_definition
) {
    bool every_path_returns = visit_code_block(function_definition.code, ScopeKind::function);
    ensure_every_path_returns_in_function_exit_navigation(every_path_returns);
}

bool FunctionExitPathNavigator::visit_code_block(
    const std::vector<Statement>& code_block, 
    ScopeKind scope_kind
) {
    bool every_path_returns = false;
    bool& code_from_now_on_is_unreachable = every_path_returns;
    for (const Statement& statement : code_block) {
        ensure_no_unreachable_code_in_function_exit_navigation(code_from_now_on_is_unreachable);
        every_path_returns = visit_statement(statement, scope_kind);
    }
    return every_path_returns;
}

bool FunctionExitPathNavigator::visit_statement(
    const Statement& statement, 
    ScopeKind scope_kind
) {
    switch (statement.statement_kind()) {
        case StatementBody::Kind::return_statement: return true;
        case StatementBody::Kind::conditional: return visit_if_statement(statement.get<Conditional>());
        case StatementBody::Kind::while_loop: return visit_while_loop(statement.get<WhileLoop>());
        case StatementBody::Kind::until_loop: return visit_until_loop(statement.get<UntilLoop>());
        case StatementBody::Kind::function_call: return false;
        case StatementBody::Kind::variable_declaration: return false;
        case StatementBody::Kind::const_declaration: return false;
        case StatementBody::Kind::assignment: return false;
        case StatementBody::Kind::break_statement: return visit_loop_blocking_statement(scope_kind);
        case StatementBody::Kind::continue_statement: return visit_loop_blocking_statement(scope_kind);
    }
}

bool FunctionExitPathNavigator::visit_loop_blocking_statement(
    ScopeKind scope_kind
) {
    bool inside_loop = scope_kind == ScopeKind::loop;
    ensure_no_loop_specific_statements_outside_loop_body_in_function_exit_navigation(inside_loop);
    return true;
}

bool FunctionExitPathNavigator::visit_if_statement(
    const Conditional& if_statement
) {
    bool then_ok = visit_code_block(if_statement.then_brench, ScopeKind::conditional);            
    bool else_ok = visit_code_block(if_statement.else_brench, ScopeKind::conditional);
    return then_ok && else_ok;
}

bool FunctionExitPathNavigator::visit_while_loop(
    const WhileLoop& while_loop
) {
    return visit_code_block(while_loop.loop_body, ScopeKind::loop);
}

bool FunctionExitPathNavigator::visit_until_loop(
    const UntilLoop& until_loop
) {
    return visit_code_block(until_loop.loop_body, ScopeKind::loop);
}