
#pragma once
#include "toolchain/tokenizer.hpp"
#include "errors/error-types.hpp"

[[nodiscard]] std::string get_sourcetext_from_tokenizer(const Tokenizer& tokenizer);

[[noreturn]] void throw_unexpected_token(const std::string& sourcetext, const Tokenizer& tokenizer);

void avoid_multiple_floating_points(bool floating, char current_char, const Tokenizer& tokenizer);

void ensure_string_gets_closed(const std::string& buffer, const Tokenizer& tokenizer);

void ensure_multiline_comments_get_closed(
    const std::stack<Token>& multiline_comments_tracker, 
    const Tokenizer& tokenizer
);
