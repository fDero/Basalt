
#pragma once
#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.h"
#include "language/typesystem.hpp"
#include "language/expressions.hpp"
#include <string>
#include <vector>

struct StatementBody : public DebugInformationsAwareEntity {

    virtual ~StatementBody() = default;
    StatementBody(const DebugInformationsAwareEntity& debug_info)
        : DebugInformationsAwareEntity(debug_info) { }
};

struct Statement : public Polymorph<StatementBody> {
    
    using Polymorph<StatementBody>::is;
    using Polymorph<StatementBody>::get;
    using Polymorph<StatementBody>::Polymorph;
};

struct VariableDeclaration : public StatementBody {
    
    virtual ~VariableDeclaration() = default;
    VariableDeclaration(
        const std::string& name, 
        const TypeSignature& type,
        const std::optional<Expression>& value,
        const Token& var_token
    );

    std::string identifier_name;
    TypeSignature typesignature;
    std::optional<Expression> initial_value;
};

struct ConstDeclaration : public StatementBody {
    
    virtual ~ConstDeclaration() = default;
    ConstDeclaration(
        const std::string& name, 
        const TypeSignature& type,
        const Expression& value,
        const Token& const_token
    );

    std::string identifier_name;
    TypeSignature typesignature;
    Expression value;
};

struct Assignment : public StatementBody {
    
    virtual ~Assignment() = default;
    Assignment(
        const Expression& target, 
        const Expression& value,
        const Token& assignment_token
    );

    Expression assignment_target;
    Expression assigned_value;
};

struct Conditional : public StatementBody {
    
    virtual ~Conditional() = default;
    Conditional(
        const Expression& cond, 
        const std::vector<Statement>& then_branch,
        const std::vector<Statement>& else_branch,
        const Token& if_token
    );

    Expression condition;
    std::vector<Statement> then_brench;
    std::vector<Statement> else_brench;
};

struct WhileLoop : public StatementBody {
    
    virtual ~WhileLoop() = default;
    WhileLoop(
        const Expression& condition, 
        const std::vector<Statement>& loop_body,
        const Token& while_token
    );

    Expression condition;
    std::vector<Statement> loop_body;
};

struct UntilLoop : public StatementBody {

    virtual ~UntilLoop() = default;
    UntilLoop(
        const Expression& condition, 
        const std::vector<Statement>& loop_body,
        const Token& until_token
    );

    Expression condition;
    std::vector<Statement> loop_body;
};

struct Return : public StatementBody {
    
    virtual ~Return() = default;
    Return(
        const std::optional<Expression>& value,
        const Token& return_token
    );

    std::optional<Expression> return_value;
};

struct Continue : public StatementBody {
    
    virtual ~Continue() = default;
    Continue(const Token& continue_token);    
};

struct Break : public StatementBody {

    virtual ~Break() = default;
    Break(const Token& break_token);  
};