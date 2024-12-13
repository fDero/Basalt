//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/statements.hpp"

VariableDeclaration::VariableDeclaration(
    const std::string& name, 
    const TypeSignature& type,
    const std::optional<Expression>& value,
    const Token& var_token
) 
    : StatementBody(var_token)
    , identifier_name(name)
    , typesignature(type)
    , initial_value(value) 
{}

ConstDeclaration::ConstDeclaration(
    const std::string& name, 
    const TypeSignature& type,
    const Expression& value,
    const Token& const_token
) 
    : StatementBody(const_token)
    , identifier_name(name)
    , typesignature(type)
    , value(value) 
{}

Assignment::Assignment(
    const Expression& target, 
    const Expression& value,
    const Token& assignment_token
)
    : StatementBody(assignment_token)
    , assignment_target(target)
    , assigned_value(value)
{}

Conditional::Conditional(
    const Expression& cond, 
    const std::vector<Statement>& then_branch,
    const std::vector<Statement>& else_branch,
    const Token& if_token
)
    : StatementBody(if_token)
    , condition(cond)
    , then_branch(then_branch)
    , else_branch(else_branch)
{}

WhileLoop::WhileLoop(
    const Expression& cond, 
    const std::vector<Statement>& body,
    const Token& while_token
)
    : StatementBody(while_token)
    , condition(cond)
    , loop_body(body) 
{}

UntilLoop::UntilLoop(
    const Expression& condition, 
    const std::vector<Statement>& loop_body,
    const Token& until_token
)
    : StatementBody(until_token)
    , condition(condition)
    , loop_body(loop_body) 
{}

Return::Return(
    const std::optional<Expression>& value,
    const Token& return_token
)
    : StatementBody(return_token)
    , return_value(value) 
{}

Continue::Continue(
    const Token& continue_token
) : StatementBody(continue_token) {}

Break::Break(
    const Token& break_token
) : StatementBody(break_token) {}