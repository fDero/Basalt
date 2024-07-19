
#pragma once
#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.h"
#include "language/typesystem.hpp"
#include "language/expressions.hpp"
#include <string>
#include <vector>

struct StatementBody : public DebugInformationsAwareEntity {

    virtual ~StatementBody() = default;

    enum class Kind {
        function_call,
        variable_declaration,
        const_declaration,
        assignment,
        conditional,
        while_loop,
        until_loop,
        return_statement,
        break_statement,
        continue_statement
    };

    [[nodiscard]] virtual StatementBody::Kind statement_kind() const = 0;

    StatementBody(const DebugInformationsAwareEntity& debug_info)
        : DebugInformationsAwareEntity(debug_info) { }
};

struct Statement : public Polymorph<StatementBody> {
    
    using Polymorph<StatementBody>::is;
    using Polymorph<StatementBody>::get;
    using Polymorph<StatementBody>::Polymorph;

    [[nodiscard]] StatementBody::Kind statement_kind() const {
		return ptr->statement_kind();
	}
};

struct VariableDeclaration : public StatementBody {
    
    virtual ~VariableDeclaration() = default;

    VariableDeclaration(
        const std::string& name, 
        const TypeSignature& type,
        const std::optional<Expression>& value,
        const Token& var_token
    );

    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::variable_declaration;
	}

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

    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::const_declaration;
	}

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

    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::assignment;
	}

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

    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::conditional;
	}

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
    
    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::while_loop;
	}

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

    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::until_loop;
	}

    Expression condition;
    std::vector<Statement> loop_body;
};

struct Return : public StatementBody {
    
    virtual ~Return() = default;
    Return(
        const std::optional<Expression>& value,
        const Token& return_token
    );

    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::return_statement;
	}

    std::optional<Expression> return_value;
};

struct Continue : public StatementBody {
    
    virtual ~Continue() = default;

    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::continue_statement;
	}

    Continue(const Token& continue_token);    
};

struct Break : public StatementBody {

    virtual ~Break() = default;

    [[nodiscard]] StatementBody::Kind statement_kind() const override {
		return StatementBody::Kind::break_statement;
	}

    Break(const Token& break_token);  
};