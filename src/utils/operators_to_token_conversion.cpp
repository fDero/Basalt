//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/token.hpp"
#include "language/expressions.hpp"
#include "misc/debug_informations_aware_entity.hpp"


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

Token TypeOperator::as_token() const {
    return Token {
        operator_text,
        DebugInformationsAwareEntity::filename,
        DebugInformationsAwareEntity::line_number,
        DebugInformationsAwareEntity::tok_number,
        DebugInformationsAwareEntity::char_pos,
        Token::Type::symbol
    };
}
