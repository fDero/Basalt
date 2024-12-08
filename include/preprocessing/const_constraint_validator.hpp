//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/program_representation.hpp"
#include "core/scope_context.hpp"
#include "preprocessing/immutability_checker.hpp"
#include "preprocessing/bond_inspector.hpp"
#include "errors/preprocessing_errors.hpp"

class ConstConstraintValidator {
    public:
        ConstConstraintValidator(ProgramRepresentation& program_representation);

        void visit_all_function_definitions();
        void visit_function_definition(const FunctionDefinition& function_definition);

    protected:
        struct SingleFunctionConstConstraintValidator {
            SingleFunctionConstConstraintValidator(
                ScopeContext& scope_context,
                ProgramRepresentation& program_representation,
                ImmutabilityChecker& immutability_checker,
                BondInspector& bond_inspector
            );

            void visit_function_definition(const FunctionDefinition& function_definition);

            void visit_expression(const Expression& statement, ScopeContext& scope_context);
            void visit_type_operator(const TypeOperator& type_operator, ScopeContext& scope_context);
            void visit_binary_operator(const BinaryOperator& binary_operator, ScopeContext& scope_context);
            void visit_unary_operator(const UnaryOperator& unary_operator, ScopeContext& scope_context);
            void visit_array_literal(const ArrayLiteral& array_literal, ScopeContext& scope_context);
            void visit_dot_member_access(const DotMemberAccess& dot_member_access, ScopeContext& scope_context);
            void visit_square_bracket_access(const SquareBracketsAccess& square_brackets_access, ScopeContext& scope_context);

            void visit_statement(const Statement& statement, ScopeContext& scope_context);
            void visit_if_statement(const Conditional& conditional, ScopeContext& scope_context);
            void visit_while_loop(const WhileLoop& while_loop, ScopeContext& scope_context);
            void visit_until_loop(const UntilLoop& until_loop, ScopeContext& scope_context);
            void visit_return_statement(const Return& return_statement, ScopeContext& scope_context);
            void visit_variable_declaration(const VariableDeclaration& variable_declaration, ScopeContext& scope_context);
            void visit_const_declaration(const ConstDeclaration& const_declaration, ScopeContext& scope_context);
            void visit_function_call(const FunctionCall& function_call, ScopeContext& scope_context);
            void visit_assignment(const Assignment& assignment, ScopeContext& scope_context);

            ScopeContext& scope_context;
            ProgramRepresentation& program_representation;
            ImmutabilityChecker& immutability_checker;
            BondInspector& bond_inspector;
        };

    private:
        ProgramRepresentation& program_representation;
};