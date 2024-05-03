
#pragma once
#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.h"
#include "language/typesystem.hpp"
#include "language/expressions.hpp"
#include <string>
#include <vector>

struct StatementBody : public DebugInformationsAwareEntity {
    
    StatementBody(const DebugInformationsAwareEntity& debug_info)
        : DebugInformationsAwareEntity(debug_info) { }

    virtual ~StatementBody() = default;
};

struct Statement : public Polymorph<StatementBody> {
    
    using Polymorph<StatementBody>::is;
    using Polymorph<StatementBody>::get;
    using Polymorph<StatementBody>::Polymorph;
};

struct VariableDeclaration : public StatementBody {
    
    std::string identifier_name;
    TypeSignature typesignature;
    std::optional<Expression> initial_value = std::nullopt;
    
    VariableDeclaration(
        const std::string& name, 
        TypeSignature& type,
        const std::optional<Expression>& value,
        const Token& var_token
    ) 
        : StatementBody(var_token)
        , identifier_name(name)
        , typesignature(type)
        , initial_value(value) 
    {}
};

struct ConstDeclaration : public StatementBody {
    
    std::string identifier_name;
    TypeSignature typesignature;
    Expression value;
    
    ConstDeclaration(
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
};

struct Assignment : public StatementBody {
    
    Expression assignment_target;
    Expression assigned_value;

    Assignment(
        const Expression& target, 
        const Expression& value,
        const Token& assignment_token
    )
        : StatementBody(assignment_token)
        , assignment_target(target)
        , assigned_value(value)
    {}
};

struct Conditional : public StatementBody {
    
    Expression condition;
    std::vector<Statement> then_brench;
    std::vector<Statement> else_brench;

    Conditional(
        const Expression& cond, 
        const std::vector<Statement>& then_brench, 
        const std::vector<Statement>& else_brench,
        const Token& if_token
    )
        : StatementBody(if_token)
        , condition(cond)
        , then_brench(then_brench)
        , else_brench(else_brench) 
    {}
};

struct WhileLoop : public StatementBody {
    
    Expression condition;
    std::vector<Statement> loop_body;

    WhileLoop(
        const Expression& condition, 
        const std::vector<Statement>& loop_body,
        const Token& while_token
    )
        : StatementBody(while_token)
        , condition(condition)
        , loop_body(loop_body) 
    {}
};

struct UntilLoop : public StatementBody {

    Expression condition;
    std::vector<Statement> loop_body;
    
    UntilLoop(
        const Expression& condition, 
        const std::vector<Statement>& loop_body,
        const Token& until_token
    )
        : StatementBody(until_token)
        , condition(condition)
        , loop_body(loop_body) 
    {}
};

struct Return : public StatementBody {
    
    std::optional<Expression> return_value;
    
    Return(
        const std::optional<Expression>& value,
        const Token& return_token
    ) 
        : StatementBody(return_token)
        , return_value(value) 
    {}
};

struct Defer : public StatementBody {

    Statement deferred_statement;
    
    Defer(
        const Statement& statement,
        const Token& defer_token
    )
        : StatementBody(defer_token)
        , deferred_statement(statement) 
    {}
};

struct Continue : public StatementBody {
    
    Continue(const Token& continue_token)
        : StatementBody(continue_token) { };    
};

struct Break : public StatementBody {

    Break(const Token& break_token)
        : StatementBody(break_token) { };  
};