/**
 * @file statements.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the different kinds of statements in the language.
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <string>
#include <vector>

#include "misc/polymorph.hpp"
#include "misc/debug_informations_aware_entity.hpp"
#include "language/typesignatures.hpp"
#include "language/expressions.hpp"

/**
 * @brief   Real base class for all the statements in the language.
 * 
 * @details Base class for all the statements in the language. Used togheter
 *          with the Statement class to create a value-semantics wrapper around
 *          the different kinds of statements in a polymorphic way.
 * 
 * @note    This struct is not meant to be used directly, instead you should use the Statement class,
 *          for wich this class is just the internal implementation.
 * 
 * @note    Since this struct extends DebugInformationsAwareEntity, all the statements in the language
 *          are aware of their position in the source code file.
 *          
 * @see     Statement
 * @see     Polymorph
 */
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

/**
 * @brief   Value-semantics polymorphism class to represent all the statements in the language.
 * 
 * @details the Statement class is used togheter with the StatementBody class to 
 *          create a value-semantics wrapper around the different kinds of statements in a polymorphic way.
 * 
 * @see     StatementBody
 * @see     Polymorph
 */
struct Statement : public Polymorph<StatementBody> {
    
    using Polymorph<StatementBody>::is;
    using Polymorph<StatementBody>::get;
    using Polymorph<StatementBody>::Polymorph;

    [[nodiscard]] StatementBody::Kind statement_kind() const;
};

/**
 * @brief   Used to represent a variable declaration in the source-code.
 * 
 * @details The VariableDeclaration struct is used to represent a variable declaration in the source-code,
 *          it contains the name of the variable, the type of the variable, and the initial value of the variable.
 */
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

/**
 * @brief   Used to represent a constant declaration in the source-code.
 * 
 * @details The ConstDeclaration struct is used to represent a constant declaration in the source-code,
 *          it contains the name of the constant, the type of the constant, and the value of the constant.
 */
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

/**
 * @brief   Used to represent an assignment in the source-code.
 * 
 * @details The Assignment struct is used to represent an assignment in the source-code,
 *          it contains the target of the assignment, the value to assign, and the assignment token.
 */
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

/**
 * @brief   Used to represent a conditional statement in the source-code.
 * 
 * @details The Conditional struct is used to represent a conditional statement in the source-code,
 *          it contains the condition of the conditional, the then branch, the else branch, and the if token.
 */
struct Conditional : public StatementBody {
    
    virtual ~Conditional() = default;

    Conditional(
        const Expression& cond, 
        const std::vector<Statement>& then_branch,
        const std::vector<Statement>& else_branch,
        const Token& if_token
    );

    [[nodiscard]] StatementBody::Kind statement_kind() const override;

    Expression condition;
    std::vector<Statement> then_brench;
    std::vector<Statement> else_brench;
};

/**
 * @brief   Used to represent a while loop in the source-code.
 * 
 * @details The WhileLoop struct is used to represent a while loop in the source-code,
 *          it contains the condition of the loop, the body of the loop, and the while token.
 */
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

/**
 * @brief   Used to represent an until loop in the source-code.
 * 
 * @details The UntilLoop struct is used to represent an until loop in the source-code,
 *          it contains the condition of the loop, the body of the loop, and the until token.
 */
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

/**
 * @brief   Used to represent a return statement in the source-code.
 * 
 * @details The Return struct is used to represent a return statement in the source-code,
 *          it contains the value to return and the return token.
 */
struct Return : public StatementBody {
    
    virtual ~Return() = default;

    Return(
        const std::optional<Expression>& value,
        const Token& return_token
    );

    [[nodiscard]] StatementBody::Kind statement_kind() const override;

    std::optional<Expression> return_value;
};

/**
 * @brief   Used to represent a continue statement in the source-code.
 * 
 * @details The Continue struct is used to represent a continue statement in the source-code,
 *          it contains the continue token.
 */
struct Continue : public StatementBody {
    
    virtual ~Continue() = default;

    [[nodiscard]] StatementBody::Kind statement_kind() const override;

    Continue(const Token& continue_token);    
};

/**
 * @brief   Used to represent a break statement in the source-code.
 * 
 * @details The Break struct is used to represent a break statement in the source-code,
 *          it contains the break token.
 */
struct Break : public StatementBody {

    virtual ~Break() = default;

    [[nodiscard]] StatementBody::Kind statement_kind() const override;

    Break(const Token& break_token);  
};