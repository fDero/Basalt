
#pragma once
#include "misc/polymorph.hpp"
#include "language/typesystem.hpp"
#include "language/expressions.hpp"
#include <string>
#include <vector>

struct StatementBody {
    /* WORK IN PROGRESS */
};

struct Statement : public Polymorph<StatementBody> {
    
    using Polymorph<StatementBody>::is;
    using Polymorph<StatementBody>::get;
    using Polymorph<StatementBody>::Polymorph;
};

struct VariableDeclaration : public StatementBody {
    std::string identifier_name;
    std::optional<TypeSignature> typesignature;
    std::optional<Expression> initial_value = std::nullopt;
    VariableDeclaration(
        const std::string& name, 
        const std::optional<TypeSignature>& type,
        const std::optional<Expression>& value
    ) : identifier_name(name), typesignature(type), initial_value(value) {}
};

struct ConstDeclaration : public StatementBody {
    std::string identifier_name;
    std::optional<TypeSignature> typesignature;
    Expression value;
    ConstDeclaration(
        const std::string& name, 
        const std::optional<TypeSignature>& type,
        const Expression& value
    ) : identifier_name(name), typesignature(type), value(value) {}
};

struct Assignment : public StatementBody {
    Expression assignment_target;
    Expression assigned_value;
    Assignment(const Expression& target, const Expression& value)
        : assignment_target(target), assigned_value(value) {}
};

struct Conditional : public StatementBody {
    Expression condition;
    std::vector<Statement> then_brench;
    std::vector<Statement> else_brench;
    Conditional(
        const Expression& cond, 
        const std::vector<Statement>& then_brench, 
        const std::vector<Statement>& else_brench
    )
        : condition(cond)
        , then_brench(then_brench)
        , else_brench(else_brench) 
    {}
};

struct WhileLoop : public StatementBody {
    Expression condition;
    std::vector<Statement> loop_body;
    WhileLoop(const Expression& condition, const std::vector<Statement>& loop_body)
        : condition(condition), loop_body(loop_body) {}
};

struct UntilLoop : public StatementBody {
    Expression condition;
    std::vector<Statement> loop_body;
    UntilLoop(const Expression& condition,const std::vector<Statement>& loop_body)
        : condition(condition), loop_body(loop_body) {}
};

struct Return : public StatementBody {
    std::optional<Expression> return_value;
    Return(const std::optional<Expression>& value) 
        : return_value(value) {}
};

struct Defer : public StatementBody {
    Statement deferred_statement;
    Defer(const Statement& statement)
        : deferred_statement(statement) {}
};

struct Continue : public StatementBody {
    /* JUST A KEYWORD */
    Continue() = default;    
};

struct Break : public StatementBody {
    /* JUST A KEYWORD */
    Break() = default;
};