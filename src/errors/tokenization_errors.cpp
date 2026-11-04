//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <stack>
#include <string>

#include "errors/compilation_error.hpp"
#include "errors/tokenization_errors.hpp"
#include "frontend/token.hpp"
#include "misc/debug_informations_aware_entity.hpp"



void throw_unexpected_token(const Token& token) {
    throw TokenizationError(
        "unexpected character" + std::string(token.sourcetext.size() > 1 ? "s" : "") +
        " `" + token.sourcetext + "`",
        token
    );
}

void avoid_multiple_floating_points(
    bool floating,
    char maybe_decimal_point,
    const DebugInformationsAwareEntity& coordinates
) {
    if (floating && maybe_decimal_point == '.') {
        throw TokenizationError(
            "a number cannot contain more than one decimal point",
            coordinates
        );
    }
}

void ensure_string_gets_closed(
    const std::string& buffer,
    const DebugInformationsAwareEntity& coordinates
) {
    if (buffer.size() < 2 || buffer.front() != buffer.back()) {
        std::string kind = (buffer.front() == '\'') ? "character literal" : "string literal";
        throw TokenizationError(
            "this " + kind + " is never closed",
            coordinates,
            std::string("add a closing ") + buffer.front() + " on the same line"
        );
    }
}

void ensure_multiline_comments_get_closed(
    const std::stack<Token>& multiline_comments_tracker,
    const DebugInformationsAwareEntity& coordinates
) {
    if (!multiline_comments_tracker.empty()) {
        const Token& comment = multiline_comments_tracker.top();
        throw TokenizationError(
            "this multiline comment is never closed",
            comment,
            "close it with `*/`"
        );
    }
}
