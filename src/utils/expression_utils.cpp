//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/expressions.hpp"

void Expression::wrap_in_parenthesis() {
    wrapped_in_parenthesis = true;
}

bool Expression::is_wrapped_in_in_parenthesis() const {
    return wrapped_in_parenthesis;
}

const DebugInformationsAwareEntity& 
Expression::as_debug_informations_aware_entity() const {
    return ptr->as_debug_informations_aware_entity();
}

bool Expression::is_literal() const {
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
