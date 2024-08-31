/**
 * @file function_definition_validator.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the FunctionDefinitionValidator class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "model/project_file_structure.hpp"
#include "model/type_definitions_register.hpp"
#include "model/function_overloads_register.hpp"
#include "model/overloading_resolution_engine.hpp"
#include "model/scope_context.hpp"

/**
 * @brief   Used to inspect the arguments section, the body and the return type of a function definition,
 *          making sure that the function is correctly defined.
 * 
 * @details The FunctionDefinitionValidator class is able to detect errors like: incorrect return value, 
 *          illegal use of 'break' and 'continue' statements, illegal use of 'return' statement, use of 
 *          undeclered identifiers, type-unsound assignments and type-unsound use of expressions as 
 *          arguments/predicates. 
 * 
 * @note    The FunctionDefinitionValidator is not able to detect missing return statements, nor it can 
 *          detect unreachable code or unused variables. It is also not capable of detecting constant-related
 *          errors, like re-assigning a constant indirectly using a pointer to its memory location 
 * 
 */
class FunctionDefinitionValidator {

    public:
        FunctionDefinitionValidator(
            ProjectFileStructure& project_file_structure,
            TypeDefinitionsRegister& type_definitions_register,
            FunctionOverloadsRegister& function_overloads_register,
            OverloadingResolutionEngine& overloading_resolution_engine
        );

        void validate_all_function_definitions();
        void validate_function_definition(const FunctionDefinition& function_definition);

    protected:
        
        void validate_function_body_statement(
            const Statement& statement, 
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_assignment(
            const Assignment& statement, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_identifier(
            const Identifier& target, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_unary_operator(
            const UnaryOperator& target, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_dereference_operator(
            const UnaryOperator& unary_operator, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_square_brackets_access(
            const SquareBracketsAccess& unary_operator, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_assignment_to_dot_member_access(
            const DotMemberAccess& unary_operator, 
            const Expression& value, 
            ScopeContext& scope_context
        );

        void validate_return_statement(
            const Return& statement,
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_conditional(
            const Conditional& statement,
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_while_loop(
            const WhileLoop& statement,
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_until_loop(
            const UntilLoop& statement,
            const FunctionDefinition& function_definition,
            ScopeContext& scope_context
        );

        void validate_function_call(
            const FunctionCall& function_call,
            ScopeContext& scope_context
        );

        void validate_variable_declaration(
            const VariableDeclaration& variable_declaration,
            ScopeContext& scope_context
        );

        void validate_const_declaratuion(
            const ConstDeclaration& const_declaration,
            ScopeContext& scope_context
        );

    private:
        ProjectFileStructure& project_file_structure;
        TypeDefinitionsRegister& type_definitions_register;
        FunctionOverloadsRegister& function_overloads_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
};
