//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/syntax.hpp"
#include "frontend/parser.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

static std::string resolve_escaped_characters(const Token& token, const std::string& source_string) {
    std::string resolved_string;
    for (size_t i = 1; i < source_string.size(); i++) {
        char prev_char = source_string[i - 1];
        char current_char = source_string[i];
        if (prev_char != '\\') { 
            resolved_string.push_back(current_char);
            continue;
        }
        switch (current_char) {
            break; case 'n':  resolved_string.back() = '\n';
            break; case 't':  resolved_string.back() = '\t';
            break; case 'r':  resolved_string.back() = '\r';
            break; case '\\': resolved_string.back() = '\\';
            break; case '\'': resolved_string.back() = '\'';
            break; case '"':  resolved_string.back() = '"';
            break; case '`':  resolved_string.back() = '`';
            default: throw_unrecognized_escape_sequence(token, current_char);
        }
    }
    return resolved_string.substr(0, resolved_string.size() - 1);
}

Expression Parser::parse_string_literal() {
    assert_token_is_string_literal(source_tokens, iterator);
    assert_string_literal_properly_formatted(iterator);
    std::string string_literal_value = resolve_escaped_characters(*iterator, iterator->sourcetext);
    StringLiteral string_literal(*iterator, string_literal_value);
    std::advance(iterator, 1);
    return string_literal;
}

Expression Parser::parse_boolean_literal() {
    assert_token_is_boolean_literal(source_tokens, iterator);
    assert_boolean_literal_properly_formatted(iterator);
    BoolLiteral bool_literal = BoolLiteral(*iterator);
    std::advance(iterator, 1);
    return bool_literal;
}

Expression Parser::parse_character_literal() {
    assert_token_is_character_literal(source_tokens, iterator);
    assert_character_literal_properly_formatted(iterator);
    std::string char_literal_value = resolve_escaped_characters(*iterator, iterator->sourcetext);
    ensure_character_literal_has_exactly_one_character(*iterator, char_literal_value);
    CharLiteral char_literal(*iterator, char_literal_value[0]);
    std::advance(iterator, 1);
    return char_literal;
}

Expression Parser::parse_floating_literal() {
    assert_token_is_floating_literal(source_tokens, iterator);
    assert_floating_literal_properly_formatted(iterator);
    FloatLiteral float_literal = FloatLiteral(*iterator);
    std::advance(iterator, 1);
    return float_literal;
}

Expression Parser::parse_integer_literal() {
    assert_token_is_integer_literal(source_tokens, iterator);
    assert_integer_literal_properly_formatted(iterator);
    IntLiteral int_literal = IntLiteral(*iterator);
    std::advance(iterator, 1);
    return int_literal;
}

Expression Parser::parse_identifier() {
    assert_token_is_text(source_tokens, iterator);
    assert_identifier_is_properly_formatted(iterator);
    Identifier identifier = Identifier (*iterator);
    std::advance(iterator, 1);
    return identifier;
}