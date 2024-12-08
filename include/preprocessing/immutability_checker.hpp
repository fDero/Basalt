//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/program_representation.hpp"
#include "core/scope_context.hpp"
#include "language/definitions.hpp"
#include "language/statements.hpp"
#include "language/functions.hpp"
#include "errors/preprocessing_errors.hpp"

class ImmutabilityChecker {

    public:
        ImmutabilityChecker(
            ScopeContext& scope_context, 
            ProgramRepresentation& program_representation
        );

        [[nodiscard]] bool is_strictly_immutable_expression(const Expression& expression);
        [[nodiscard]] bool is_identifier_immutable(const Identifier& identifier);
        [[nodiscard]] bool is_unary_operator_immutable(const UnaryOperator& unary_operator);
        [[nodiscard]] bool is_square_bracket_access_immutable(const SquareBracketsAccess& square_brackets_access);
        [[nodiscard]] bool is_dot_member_access_immutable(const DotMemberAccess& dot_member_access);
        [[nodiscard]] bool is_type_operator_immutable(const TypeOperator& type_operator);

        [[nodiscard]] bool is_weakly_immutable_expression(const Expression& expression);
        [[nodiscard]] bool is_function_call_weakly_immutable(const FunctionCall& function_call);
        [[nodiscard]] bool is_unary_weakly_operator_immutable(const UnaryOperator& unary_operator);
        
    private:
        ScopeContext& scope_context;
        ProgramRepresentation& program_representation;
};