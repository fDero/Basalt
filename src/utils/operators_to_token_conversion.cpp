//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "language/expressions.hpp"
#include "frontend/tokenizer.hpp"

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