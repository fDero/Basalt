//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "language/typesignatures.hpp"
#include "language/expressions.hpp"
#include "core/expression_type_deducer.hpp"
#include "frontend/syntax.hpp"

class DotMemberAccessNormalizer {
    public:
        DotMemberAccessNormalizer(
            ExpressionTypeDeducer& expression_type_deducer,
            const DotMemberAccess& dot_member_access
        )
            : expression_type_deducer(expression_type_deducer)
            , dot_member_access(dot_member_access)
        {}

        [[nodiscard]] DotMemberAccess normalize_dot_member_access() {
            Expression normalized_target = normalize_dot_member_access_target(dot_member_access.struct_value);
            return DotMemberAccess(
                dot_member_access.as_debug_informations_aware_entity(),
                normalized_target,
                dot_member_access.member_name
            );
        }

    protected:
        [[nodiscard]] Expression normalize_dot_member_access_target(const Expression& target) {
            std::optional<TypeSignature> target_type_opt = expression_type_deducer.deduce_expression_type(target);
            if (target_type_opt.has_value() && target_type_opt.value().is<PointerType>()) {
                UnaryOperator maybe_normalized_target(
                    dot_member_access.as_debug_informations_aware_entity(), 
                    pointer_dereference_operator,
                    target
                );
                return normalize_dot_member_access_target(maybe_normalized_target);
            }
            return target;
        }        

    private:
        ExpressionTypeDeducer& expression_type_deducer;
        const DotMemberAccess& dot_member_access;
};
