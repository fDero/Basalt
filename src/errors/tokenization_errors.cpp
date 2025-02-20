//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/tokenization_errors.hpp"
#include "syntax/specials.hpp"
using CompilationError::Kind::TokenizationError;

void throw_unexpected_token(const Token& token) {
    CompilationError::raise<TokenizationError>(
        "unexpected token: " + token.sourcetext, 
        token
    );
}

void avoid_multiple_floating_points(
    bool floating, 
    char maybe_decimal_point,
    const DebugInformationsAwareEntity& coordinates
) {
    if (floating && maybe_decimal_point == '.') {
        CompilationError::raise<TokenizationError>(
            "multiple decimal points in the same number", 
            coordinates
        );
    }
}

void ensure_string_gets_closed(
    const std::string& buffer, 
    const DebugInformationsAwareEntity& coordinates
) {
    if (buffer.front() != buffer.back()) {
        CompilationError::raise<TokenizationError>(
            "string/char literal opened but never closed", 
            coordinates
        );
    }
}

void ensure_multiline_comments_get_closed(
    const std::stack<Token>& multiline_comments_tracker, 
    const DebugInformationsAwareEntity& coordinates
) {
    if (!multiline_comments_tracker.empty()) {
        const Token& comment = multiline_comments_tracker.top();
        CompilationError::raise<TokenizationError>(
            "multiline comment opened but never closed",
            comment
        );
    }
}