//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/generics_substitution_rules.hpp"
#include "core/generics_instantiation_engine.hpp"
#include "language/statements.hpp"
#include "errors/internal_errors.hpp"

Statement GenericsInstantiationEngine::instantiate_generic_statement(const Statement& statement) {
    switch (statement.statement_kind()) {
        case StatementBody::Kind::function_call: return instantiate_generic_function_call(statement.get<FunctionCall>());
        case StatementBody::Kind::variable_declaration: return instantiate_generic_variable_declaration(statement.get<VariableDeclaration>()); 
        case StatementBody::Kind::const_declaration: return instantiate_generic_const_declaration(statement.get<ConstDeclaration>());
        case StatementBody::Kind::assignment: return instantiate_generic_assignment(statement.get<Assignment>());
        case StatementBody::Kind::conditional: return instantiate_generic_conditional(statement.get<Conditional>());
        case StatementBody::Kind::while_loop: return instantiate_generic_while_loop(statement.get<WhileLoop>());
        case StatementBody::Kind::until_loop: return instantiate_generic_until_loop(statement.get<UntilLoop>());
        case StatementBody::Kind::return_statement: return statement;
        case StatementBody::Kind::continue_statement: return statement;
        case StatementBody::Kind::break_statement: return statement;
    }
    assert_unreachable();
}

VariableDeclaration GenericsInstantiationEngine::instantiate_generic_variable_declaration(const VariableDeclaration& statement) {
    VariableDeclaration instantiated_variable_declaration = statement;
    instantiated_variable_declaration.typesignature = instantiate_generic_typesignature(statement.typesignature);
    if (statement.initial_value.has_value()) {
        instantiated_variable_declaration.initial_value = instantiate_generic_expression(statement.initial_value.value());
    }
    return instantiated_variable_declaration;
}

ConstDeclaration GenericsInstantiationEngine::instantiate_generic_const_declaration(const ConstDeclaration& statement) {
    ConstDeclaration instantiated_const_declaration = statement;
    instantiated_const_declaration.typesignature = instantiate_generic_typesignature(statement.typesignature);
    instantiated_const_declaration.value = instantiate_generic_expression(statement.value);
    return instantiated_const_declaration;
}

Assignment GenericsInstantiationEngine::instantiate_generic_assignment(const Assignment& statement) {
    Assignment instantiated_assignment = statement;
    instantiated_assignment.assignment_target = instantiate_generic_expression(statement.assignment_target);
    instantiated_assignment.assigned_value = instantiate_generic_expression(statement.assigned_value);
    return instantiated_assignment;
}

Conditional GenericsInstantiationEngine::instantiate_generic_conditional(const Conditional& statement) {
    Conditional instantiated_conditional = statement;
    instantiated_conditional.condition = instantiate_generic_expression(statement.condition);
    for (Statement& statement : instantiated_conditional.then_branch) {
        statement = instantiate_generic_statement(statement);
    }
    for (Statement& statement : instantiated_conditional.else_branch) {
        statement = instantiate_generic_statement(statement);
    }
    return instantiated_conditional;
}

WhileLoop GenericsInstantiationEngine::instantiate_generic_while_loop(const WhileLoop& statement) {
    WhileLoop instantiated_while_loop = statement;
    instantiated_while_loop.condition = instantiate_generic_expression(statement.condition);
    for (Statement& statement : instantiated_while_loop.loop_body) {
        statement = instantiate_generic_statement(statement);
    }
    return instantiated_while_loop;
}

UntilLoop GenericsInstantiationEngine::instantiate_generic_until_loop(const UntilLoop& statement) {
    UntilLoop instantiated_until_loop = statement;
    instantiated_until_loop.condition = instantiate_generic_expression(statement.condition);
    for (Statement& statement : instantiated_until_loop.loop_body) {
        statement = instantiate_generic_statement(statement);
    }
    return instantiated_until_loop;
}

Return GenericsInstantiationEngine::instantiate_generic_return_statement(const Return& statement) {
    if (!statement.return_value.has_value()) {
        return statement;
    }
    Return instantiated_return = statement;
    instantiated_return.return_value = instantiate_generic_expression(statement.return_value.value());
    return instantiated_return;
}