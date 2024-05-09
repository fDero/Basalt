
#include "language/expressions.hpp"

Token UnaryOperator::as_token() const {
    return Token { 
        operator_text, 
        DebugInformationsAwareEntity::filename,
        DebugInformationsAwareEntity::line_number,
        DebugInformationsAwareEntity::tok_number,
        DebugInformationsAwareEntity::char_pos,
        Token::Type::symbol
    };
}

Token BinaryOperator::as_token() const {
    return Token { 
        operator_text, 
        DebugInformationsAwareEntity::filename,
        DebugInformationsAwareEntity::line_number,
        DebugInformationsAwareEntity::tok_number,
        DebugInformationsAwareEntity::char_pos,
        Token::Type::symbol
    };
}