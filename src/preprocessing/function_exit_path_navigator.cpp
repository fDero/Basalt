//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/function_exit_path_navigator.hpp"
#include "errors/internal_errors.hpp"

#define EPK FunctionExitPathNavigator::ExitPathKind

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
    EPK codeblock_exit = visit_code_block(function_definition.code, ScopeKind::function);
    bool every_path_returns = (codeblock_exit == EPK::definitive_exit);
    if (function_definition.return_type.has_value()) {
        ensure_every_path_returns_in_function_exit_navigation(every_path_returns);
    }
}

EPK FunctionExitPathNavigator::visit_code_block(
    const std::vector<Statement>& code_block, 
    ScopeKind scope_kind
) {
    EPK exit_path_found = EPK::no_exit;
    bool code_from_now_on_is_unreachable = false;
    for (const Statement& statement : code_block) {
        ensure_no_unreachable_code_in_function_exit_navigation(code_from_now_on_is_unreachable);
        exit_path_found = visit_statement(statement, scope_kind);
        code_from_now_on_is_unreachable = (exit_path_found != EPK::no_exit);
    }
    return exit_path_found;
}

EPK FunctionExitPathNavigator::visit_statement(
    const Statement& statement, 
    ScopeKind scope_kind
) {
    switch (statement.statement_kind()) {
        case StatementBody::Kind::return_statement: return EPK::definitive_exit;
        case StatementBody::Kind::conditional: return visit_if_statement(statement.get<Conditional>(), scope_kind);
        case StatementBody::Kind::while_loop: return visit_while_loop(statement.get<WhileLoop>());
        case StatementBody::Kind::until_loop: return visit_until_loop(statement.get<UntilLoop>());
        case StatementBody::Kind::function_call: return EPK::no_exit;
        case StatementBody::Kind::variable_declaration: return EPK::no_exit;
        case StatementBody::Kind::const_declaration: return EPK::no_exit;
        case StatementBody::Kind::assignment: return EPK::no_exit;
        case StatementBody::Kind::break_statement: return visit_loop_blocking_statement(scope_kind);
        case StatementBody::Kind::continue_statement: return visit_loop_blocking_statement(scope_kind);
    }
    assert_unreachable();
}

EPK FunctionExitPathNavigator::visit_loop_blocking_statement(
    ScopeKind scope_kind
) {
    bool inside_loop = scope_kind == ScopeKind::loop;
    ensure_no_loop_specific_statements_outside_loop_body_in_function_exit_navigation(inside_loop);
    return EPK::local_control_flow_alteration;
}

EPK FunctionExitPathNavigator::visit_if_statement(
    const Conditional& if_statement, 
    ScopeKind scope_kind
) {
    EPK then_ok = visit_code_block(if_statement.then_branch, scope_kind);            
    EPK else_ok = visit_code_block(if_statement.else_branch, scope_kind);
    if (then_ok == EPK::definitive_exit && else_ok == EPK::definitive_exit) {
        return EPK::definitive_exit;
    }
    else if (then_ok == EPK::no_exit || else_ok == EPK::no_exit) {
        return EPK::no_exit;
    }
    else {
        return EPK::local_control_flow_alteration;
    }
}

EPK FunctionExitPathNavigator::visit_while_loop(
    const WhileLoop& while_loop
) {
    std::ignore = visit_code_block(while_loop.loop_body, ScopeKind::loop);
    return EPK::no_exit;
}

EPK FunctionExitPathNavigator::visit_until_loop(
    const UntilLoop& until_loop
) {
    EPK exit_kind = visit_code_block(until_loop.loop_body, ScopeKind::loop);
    return (exit_kind == EPK::local_control_flow_alteration) 
        ? EPK::no_exit 
        : exit_kind;
}