//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/tokenization_errors.hpp"
#include "frontend/syntax.hpp"

std::string get_sourcetext_from_tokenizer(const Tokenizer& tokenizer) {
    std::string extracted;
    std::string current_line = tokenizer.get_current_line();
    for (size_t i = tokenizer.get_char_pos(); i < current_line.size(); i++) {
        if (discardable.find(current_line[i]) != discardable.end()) break;
        extracted.push_back(current_line[i]);
    }
    return extracted;
}

[[noreturn]] void throw_unexpected_token(const std::string& sourcetext, const Tokenizer& tokenizer) {
    throw TokenizationError {
            "unexpected token: " + sourcetext, sourcetext, 
            tokenizer.get_filename(), tokenizer.get_line_number(), 
            tokenizer.get_tok_number(), tokenizer.get_char_pos()
        };
}

void avoid_multiple_floating_points(bool floating, char current_char, const Tokenizer& tokenizer) {
    if (floating && current_char == '.') {
        throw TokenizationError {
            "multiple floating points in the same number", 
            get_sourcetext_from_tokenizer(tokenizer), 
            tokenizer.get_filename(), tokenizer.get_line_number(), 
            tokenizer.get_tok_number(), tokenizer.get_char_pos()
        };
    }
}

void ensure_string_gets_closed(const std::string& buffer, const Tokenizer& tokenizer) {
    std::string current_line = tokenizer.get_current_line();
    unsigned int char_pos = tokenizer.get_char_pos();
    if (current_line[char_pos+buffer.size()] != current_line[char_pos]) {
        throw TokenizationError {
            "string opened but never closed", 
            get_sourcetext_from_tokenizer(tokenizer), 
            tokenizer.get_filename(), tokenizer.get_line_number(), 
            tokenizer.get_tok_number(), tokenizer.get_char_pos()
        };
    }
}

void ensure_multiline_comments_get_closed(
    const std::stack<Token>& multiline_comments_tracker, 
    const Tokenizer& tokenizer
) {
    if (!multiline_comments_tracker.empty()) {
        const Token& comment = multiline_comments_tracker.top();
        throw TokenizationError {
            "multiline comment opened but never closed", "/*",
            tokenizer.get_filename(), comment.line_number, 
            comment.tok_number, comment.char_pos
        };
    }
}
