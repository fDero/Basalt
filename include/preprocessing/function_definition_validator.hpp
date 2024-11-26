//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/project_file_structure.hpp"
#include "core/type_definitions_register.hpp"
#include "core/function_overloads_register.hpp"
#include "core/overloading_resolution_engine.hpp"
#include "core/scope_context.hpp"

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

        void validate_const_declaration(
            const ConstDeclaration& const_declaration,
            ScopeContext& scope_context
        );

    private:
        ProjectFileStructure& project_file_structure;
        TypeDefinitionsRegister& type_definitions_register;
        FunctionOverloadsRegister& function_overloads_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
};
