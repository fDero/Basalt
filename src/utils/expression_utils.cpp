
#include "language/expressions.hpp"

void Expression::wrap_in_parenthesis() {
    wrapped_in_parenthesis = true;
}

[[nodiscard]] bool Expression::is_wrapped_in_in_parenthesis() const {
    return wrapped_in_parenthesis;
}

[[nodiscard]] const DebugInformationsAwareEntity& 
Expression::as_debug_informations_aware_entity() const {
    return ptr->as_debug_informations_aware_entity();
}

[[nodiscard]] bool Expression::is_literal() const {
    switch (expression_kind()) {
        case ExpressionBody::Kind::int_literal: 
        case ExpressionBody::Kind::float_literal:
        case ExpressionBody::Kind::string_literal:
        case ExpressionBody::Kind::char_literal:
        case ExpressionBody::Kind::bool_literal:
        case ExpressionBody::Kind::array_literal:
            return true;
        default:
            return false;
    }
}
