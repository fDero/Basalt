//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "language/expressions.hpp"
#include "language/typesignatures.hpp"
#include "misc/debug_informations_aware_entity.hpp"
#include "misc/forward_declarations.hpp"
#include "misc/polymorph.hpp"


struct StatementBody : public DebugInformationsAwareEntity {

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

    virtual ~StatementBody() = default;
    StatementBody(const DebugInformationsAwareEntity& debug_info)
        : DebugInformationsAwareEntity(debug_info) { }

    [[nodiscard]] virtual StatementBody::Kind statement_kind() const = 0;
};

struct Statement : public Polymorph<StatementBody> {

    using Polymorph<StatementBody>::is;
    using Polymorph<StatementBody>::get;
    using Polymorph<StatementBody>::Polymorph;

    [[nodiscard]] StatementBody::Kind statement_kind() const;
    [[nodiscard]] const DebugInformationsAwareEntity& as_debug_informations_aware_entity() const;
};

struct VariableDeclaration : public StatementBody {

    virtual ~VariableDeclaration() = default;
    VariableDeclaration(
        const std::string& name,
        const TypeSignature& type,
        const std::optional<Expression>& value,
        const Token& var_token
    );

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
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

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
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

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
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

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
    std::vector<Statement> then_branch;
    std::vector<Statement> else_branch;
    Expression condition;
};

struct WhileLoop : public StatementBody {

    virtual ~WhileLoop() = default;
    WhileLoop(
        const Expression& condition,
        const std::vector<Statement>& loop_body,
        const Token& while_token
    );

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
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

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
    Expression condition;
    std::vector<Statement> loop_body;
};

struct Return : public StatementBody {

    virtual ~Return() = default;
    Return(
        const std::optional<Expression>& value,
        const Token& return_token
    );

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
    std::optional<Expression> return_value;
};

struct Continue : public StatementBody {

    virtual ~Continue() = default;
    Continue(const Token& continue_token);

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
};

struct Break : public StatementBody {

    virtual ~Break() = default;
    Break(const Token& break_token);

    [[nodiscard]] StatementBody::Kind statement_kind() const override;
};
