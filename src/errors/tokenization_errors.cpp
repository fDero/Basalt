//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/tokenization_errors.hpp"
#include "syntax/specials.hpp"

[[noreturn]] void throw_unexpected_token(const Token& token) {
    throw TokenizationError {
        "unexpected token: " + token.sourcetext, 
        token.sourcetext, 
        token.filename, 
        token.line_number, 
        token.tok_number, 
        token.char_pos
    };
}

void avoid_multiple_floating_points(
    bool floating, 
    char maybe_decimal_point,
    const DebugInformationsAwareEntity& coordinates
) {
    if (floating && maybe_decimal_point == '.') {
        throw TokenizationError {
            "multiple floating points in the same number", 
            std::string{maybe_decimal_point}, 
            coordinates.filename,
            coordinates.line_number,
            coordinates.tok_number,
            coordinates.char_pos
        };
    }
}

void ensure_string_gets_closed(
    const std::string& buffer, 
    const DebugInformationsAwareEntity& coordinates
) {
    if (buffer.front() != buffer.back()) {
        throw TokenizationError {
            "string/char literal opened but never closed", 
            std::string{buffer.front()}, 
            coordinates.filename, 
            coordinates.line_number, 
            coordinates.tok_number, 
            coordinates.char_pos
        };
    }
}

void ensure_multiline_comments_get_closed(
    const std::stack<Token>& multiline_comments_tracker, 
    const DebugInformationsAwareEntity& coordinates
) {
    if (!multiline_comments_tracker.empty()) {
        const Token& comment = multiline_comments_tracker.top();
        throw TokenizationError {
            "multiline comment opened but never closed",
            comment.sourcetext,
            coordinates.filename, 
            comment.line_number, 
            comment.tok_number, 
            comment.char_pos
        };
    }
}
