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
        )
            : scope_context(scope_context)
            , program_representation(program_representation) 
        {}

        bool is_weakly_immutable_expression(const Expression& expression) {
            switch (expression.expression_kind()) {
                case ExpressionBody::Kind::function_call: 
                    return is_function_call_weakly_immutable(expression.get<FunctionCall>());
                default: 
                    return is_strictly_immutable_expression(expression);
            }
        }

        [[nodiscard]] bool is_strictly_immutable_expression(const Expression& expression) {
            switch (expression.expression_kind()) {
                case ExpressionBody::Kind::identifier:            return is_identifier_immutable(expression.get<Identifier>());
                case ExpressionBody::Kind::type_operator:         return is_type_operator_immutable(expression.get<TypeOperator>());
                case ExpressionBody::Kind::dot_member_access:     return is_dot_member_access_immutable(expression.get<DotMemberAccess>());
                case ExpressionBody::Kind::square_bracket_access: return is_square_bracket_access_immutable(expression.get<SquareBracketsAccess>());
                case ExpressionBody::Kind::unary_operator:        return is_unary_operator_immutable(expression.get<UnaryOperator>());
                case ExpressionBody::Kind::function_call:         return true;
                case ExpressionBody::Kind::binary_operator:       return true;
                case ExpressionBody::Kind::array_literal:         return true;
                case ExpressionBody::Kind::string_literal:        return true;
                case ExpressionBody::Kind::float_literal:         return true;
                case ExpressionBody::Kind::char_literal:          return true;
                case ExpressionBody::Kind::int_literal:           return true;
                case ExpressionBody::Kind::bool_literal:          return true;
            }
        }

        [[nodiscard]] bool is_identifier_immutable(const Identifier& identifier) {
            scope_context.is_identifier_immutable(identifier.name);
        }

        [[nodiscard]] bool is_unary_operator_immutable(const UnaryOperator& unary_operator) {
            return unary_operator.operator_text != "#";
        }

        [[nodiscard]] bool is_square_bracket_access_immutable(const SquareBracketsAccess& square_brackets_access) {
            return is_weakly_immutable_expression(square_brackets_access.storage);
        }

        [[nodiscard]] bool is_dot_member_access_immutable(const DotMemberAccess& dot_member_access) {
            return is_weakly_immutable_expression(dot_member_access.struct_value);
        }

        [[nodiscard]] bool is_type_operator_immutable(const TypeOperator& type_operator) {
            return type_operator.operator_text == "as" && is_weakly_immutable_expression(type_operator.expression);
        }

        [[nodiscard]] bool is_function_call_weakly_immutable(const FunctionCall& function_call) {
            std::optional<TypeSignature> return_type = program_representation.resolve_expression_type(function_call, scope_context);
            return !return_type.has_value() || (!return_type->is<PointerType>() && !return_type->is<SliceType>());
        }

    private:
        ScopeContext& scope_context;
        ProgramRepresentation& program_representation;
};