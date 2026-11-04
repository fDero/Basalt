//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "misc/forward_declarations.hpp"


class ImmutabilityDeducer {

    public:
        ImmutabilityDeducer(
            ScopeContext& scope_context,
            ProgramRepresentation& program_representation
        );

        [[nodiscard]] bool is_expression_immutable(const Expression& expression);
        [[nodiscard]] bool is_identifier_immutable(const Identifier& identifier);
        [[nodiscard]] bool is_unary_operator_immutable(const UnaryOperator& unary_operator);
        [[nodiscard]] bool is_square_bracket_access_immutable(const SquareBracketsAccess& square_brackets_access);
        [[nodiscard]] bool is_dot_member_access_immutable(const DotMemberAccess& dot_member_access);
        [[nodiscard]] bool is_type_operator_immutable(const TypeOperator& type_operator);
        [[nodiscard]] bool is_function_call_immutable(const FunctionCall& function_call);

    private:
        ScopeContext& scope_context;
        ProgramRepresentation& program_representation;
};
