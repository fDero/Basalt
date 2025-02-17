//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/program_representation.hpp"
#include "core/scope_context.hpp"
#include "language/expressions.hpp"
#include "errors/preprocessing_errors.hpp"

class ObservabilityDeducer {

    public:
        ObservabilityDeducer(
            ScopeContext& scope_context, 
            ProgramRepresentation& program_representation
        );

        [[nodiscard]] bool is_expression_update_observable(const Expression& expression);
        [[nodiscard]] bool is_unary_operator_update_observable(const UnaryOperator& unary_operator);
        [[nodiscard]] bool is_square_bracket_access_update_observable(const SquareBracketsAccess& square_brackets_access);
        [[nodiscard]] bool is_dot_member_access_update_observable(const DotMemberAccess& dot_member_access);
        [[nodiscard]] bool is_type_operator_update_observable(const TypeOperator& type_operator);
        [[nodiscard]] bool is_function_call_update_observable(const FunctionCall& function_call);

    private:
        ScopeContext& scope_context;
        ProgramRepresentation& program_representation;
};