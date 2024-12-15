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

class AddressSanitizer {
    public:
        AddressSanitizer(ProgramRepresentation& program_representation);

        void visit_all_function_definitions();
        void visit_function_definition(const FunctionDefinition& function_definition);

    protected:
        struct SingleFunctionAddressSanitizer {
            SingleFunctionAddressSanitizer(
                ScopeContext& scope_context,
                ProgramRepresentation& program_representation
            );

            void visit_function_definition(const FunctionDefinition& function_definition);
            void visit_code_block(const std::vector<Statement>& code_block);

            void visit_expression(const Expression& statement);
            void visit_type_operator(const TypeOperator& type_operator);
            void visit_binary_operator(const BinaryOperator& binary_operator);
            void visit_unary_operator(const UnaryOperator& unary_operator);
            void visit_array_literal(const ArrayLiteral& array_literal);
            void visit_dot_member_access(const DotMemberAccess& dot_member_access);
            void visit_square_bracket_access(const SquareBracketsAccess& square_brackets_access);

            void visit_statement(const Statement& statement);
            void visit_conditional(const Conditional& conditional);
            void visit_while_loop(const WhileLoop& while_loop);
            void visit_until_loop(const UntilLoop& until_loop);
            void visit_return_statement(const Return& return_statement);
            void visit_variable_declaration(const VariableDeclaration& variable_declaration);
            void visit_const_declaration(const ConstDeclaration& const_declaration);
            void visit_function_call(const FunctionCall& function_call);
            void visit_assignment(const Assignment& assignment);

            [[nodiscard]] bool is_expression_addressable(const Expression& expr);

            ScopeContext& scope_context;
            ProgramRepresentation& program_representation;
        };

    private:
        ProgramRepresentation& program_representation;
};