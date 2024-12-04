//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/program_representation.hpp"
#include "core/scope_context.hpp"
#include "preprocessing/immutability_checker.hpp"
#include "preprocessing/bound_inspector.hpp"
#include "errors/preprocessing_errors.hpp"

class ConstConstraintValidator {
    public:
        ConstConstraintValidator(ProgramRepresentation& program_representation)
            : program_representation(program_representation) {}

        void visit_all_function_definitions() {
            program_representation.foreach_function_definition([&](const FunctionDefinition::Ref& function_definition) {
                visit_function_definition(*function_definition);
            });
        }

        void visit_function_definition(const FunctionDefinition& function_definition) {
            ScopeContext scope_context(function_definition.arguments);
            ImmutabilityChecker immutability_checker(scope_context, program_representation);
            BoundInspector bound_inspector(scope_context, program_representation);
            SingleFunctionConstConstraintValidator single_function_const_constraint_validator(
                program_representation,
                immutability_checker,
                bound_inspector
            );
            single_function_const_constraint_validator.visit_function_definition(function_definition);
        }

    protected:
        struct SingleFunctionConstConstraintValidator {

            SingleFunctionConstConstraintValidator(
                ProgramRepresentation& program_representation,
                ImmutabilityChecker& immutability_checker,
                BoundInspector& bound_inspector
            )
                : program_representation(program_representation)
                , immutability_checker(immutability_checker)
                , bound_inspector(bound_inspector)
            {}

            void visit_function_definition(const FunctionDefinition& function_definition) {
                ScopeContext scope_context(function_definition.arguments);
                for (const Statement& statement : function_definition.code) {
                    visit_statement(statement, scope_context);
                }
            }

            void visit_expression(const Expression& statement, ScopeContext& scope_context) {
                switch (statement.expression_kind()) {
                    break; case ExpressionBody::Kind::function_call:         visit_function_call(statement.get<FunctionCall>(), scope_context);
                    break; case ExpressionBody::Kind::type_operator:         visit_type_operator(statement.get<TypeOperator>(), scope_context);
                    break; case ExpressionBody::Kind::binary_operator:       visit_binary_operator(statement.get<BinaryOperator>(), scope_context);
                    break; case ExpressionBody::Kind::unary_operator:        visit_unary_operator(statement.get<UnaryOperator>(), scope_context);
                    break; case ExpressionBody::Kind::array_literal:         visit_array_literal(statement.get<ArrayLiteral>(), scope_context);
                    break; case ExpressionBody::Kind::dot_member_access:     visit_dot_member_access(statement.get<DotMemberAccess>(), scope_context);
                    break; case ExpressionBody::Kind::square_bracket_access: visit_square_bracket_access(statement.get<SquareBracketsAccess>(), scope_context);
                    break; case ExpressionBody::Kind::identifier:            return;
                    break; case ExpressionBody::Kind::string_literal:        return;
                    break; case ExpressionBody::Kind::float_literal:         return;
                    break; case ExpressionBody::Kind::char_literal:          return;
                    break; case ExpressionBody::Kind::int_literal:           return;
                    break; case ExpressionBody::Kind::bool_literal:          return;
                }
            }

            void visit_type_operator(const TypeOperator& type_operator, ScopeContext& scope_context) {
                visit_expression(type_operator.expression, scope_context);
            }

            void visit_binary_operator(const BinaryOperator& binary_operator, ScopeContext& scope_context) {
                visit_expression(binary_operator.left_operand, scope_context);
                visit_expression(binary_operator.right_operand, scope_context);
            }

            void visit_unary_operator(const UnaryOperator& unary_operator, ScopeContext& scope_context) {
                visit_expression(unary_operator.operand, scope_context);
            }

            void visit_array_literal(const ArrayLiteral& array_literal, ScopeContext& scope_context) {
                for (const Expression& element : array_literal.elements) {
                    visit_expression(element, scope_context);
                }
            }

            void visit_dot_member_access(const DotMemberAccess& dot_member_access, ScopeContext& scope_context) {
                visit_expression(dot_member_access.struct_value, scope_context);
            }

            void visit_square_bracket_access(const SquareBracketsAccess& square_brackets_access, ScopeContext& scope_context) {
                visit_expression(square_brackets_access.storage, scope_context);
            }

            void visit_statement(const Statement& statement, ScopeContext& scope_context) {
                switch (statement.statement_kind()) {
                    break; case StatementBody::Kind::return_statement: visit_return_statement(statement.get<Return>(), scope_context);
                    break; case StatementBody::Kind::conditional: visit_if_statement(statement.get<Conditional>(), scope_context);
                    break; case StatementBody::Kind::while_loop: visit_while_loop(statement.get<WhileLoop>(), scope_context);
                    break; case StatementBody::Kind::until_loop: visit_until_loop(statement.get<UntilLoop>(), scope_context);
                    break; case StatementBody::Kind::function_call: visit_function_call(statement.get<FunctionCall>(), scope_context);
                    break; case StatementBody::Kind::variable_declaration: visit_variable_declaration(statement.get<VariableDeclaration>(), scope_context);
                    break; case StatementBody::Kind::const_declaration: visit_const_declaration(statement.get<ConstDeclaration>(), scope_context);
                    break; case StatementBody::Kind::assignment: visit_assignment(statement.get<Assignment>(), scope_context);
                    break; case StatementBody::Kind::break_statement: return;
                    break; case StatementBody::Kind::continue_statement: return;
                }
            }

            void visit_if_statement(const Conditional& conditional, ScopeContext& scope_context) {
                visit_expression(conditional.condition, scope_context);
                ScopeContext then_scope_context = scope_context.create_nested_scope();
                ScopeContext else_scope_context = scope_context.create_nested_scope();
                for (const Statement& statement : conditional.then_brench) {
                    visit_statement(statement, then_scope_context);
                }
                for (const Statement& statement : conditional.else_brench) {
                    visit_statement(statement, else_scope_context);
                }
            }

            void visit_while_loop(const WhileLoop& while_loop, ScopeContext& scope_context) {
                visit_expression(while_loop.condition, scope_context);
                ScopeContext loop_scope_context = scope_context.create_nested_scope();
                for (const Statement& statement : while_loop.loop_body) {
                    visit_statement(statement, loop_scope_context);
                }
            }

            void visit_until_loop(const UntilLoop& until_loop, ScopeContext& scope_context) {
                visit_expression(until_loop.condition, scope_context);
                ScopeContext loop_scope_context = scope_context.create_nested_scope();
                for (const Statement& statement : until_loop.loop_body) {
                    visit_statement(statement, loop_scope_context);
                }
            }

            void visit_return_statement(const Return& return_statement, ScopeContext& scope_context) {
                if (return_statement.return_value.has_value()) {
                    visit_expression(return_statement.return_value.value(), scope_context);
                }
            }

            void visit_variable_declaration(const VariableDeclaration& variable_declaration, ScopeContext& scope_context) {
                if (variable_declaration.initial_value.has_value()) {
                    visit_expression(variable_declaration.initial_value.value(), scope_context);
                }
                scope_context.store_local_variable(variable_declaration);
            }

            void visit_const_declaration(const ConstDeclaration& const_declaration, ScopeContext& scope_context) {
                visit_expression(const_declaration.value, scope_context);
                scope_context.store_local_constant(const_declaration);
            }

            void visit_function_call(const FunctionCall& function_call, ScopeContext& scope_context) {
                for (const Expression& argument : function_call.arguments) {
                    visit_expression(argument, scope_context);
                    bool assignment_of_immutable_value = immutability_checker.is_strictly_immutable_expression(argument);
                    std::optional<TypeSignature> type = program_representation.resolve_expression_type(argument, scope_context);
                    bool assignment_implies_bound = type.has_value() && bound_inspector.does_the_type_of_this_expr_imply_a_bound(*type);
                    bool assignment_discard_qualifiers = assignment_of_immutable_value && assignment_implies_bound;
                    ensure_use_as_function_argument_complies_with_const_qualifiers(function_call, argument, assignment_discard_qualifiers);
                }
            }

            void visit_assignment(const Assignment& assignment, ScopeContext& scope_context) {
                visit_expression(assignment.assigned_value, scope_context);
                bool assignment_to_immutable_target = immutability_checker.is_strictly_immutable_expression(assignment.assignment_target);
                bool assignment_of_immutable_value = immutability_checker.is_strictly_immutable_expression(assignment.assigned_value);
                std::optional<TypeSignature> type = program_representation.resolve_expression_type(assignment.assigned_value, scope_context);
                bool assignment_implies_bound = type.has_value() && bound_inspector.does_the_type_of_this_expr_imply_a_bound(*type);
                bool assignment_discard_qualifiers = assignment_to_immutable_target || (assignment_of_immutable_value && assignment_implies_bound);
                ensure_assignment_complies_with_const_qualifiers(assignment, assignment_discard_qualifiers);
            }

            ProgramRepresentation& program_representation;
            ImmutabilityChecker& immutability_checker;
            BoundInspector& bound_inspector;
        };

    private:
        ProgramRepresentation& program_representation;
};