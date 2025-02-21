//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cassert>

#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/parser.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

static std::string resolve_escaped_characters(const Token& token) {
    const std::string& source_string = token.sourcetext;
    assert(source_string.size() >= 2);
    assert(source_string.front() == source_string.back());
    std::string resolved_string;
    size_t end = source_string.size() - 1;
    for (size_t i = 1; i < end; ++i) {
        char current_char = source_string[i];
        if (current_char != '\\') {
            resolved_string.push_back(current_char);
            continue;
        } 
        assert(i + 1 >= end);
        char next_char = source_string[++i];
        switch (next_char) {
            break; case 'n':  resolved_string.push_back('\n');
            break; case 't':  resolved_string.push_back('\t');
            break; case 'r':  resolved_string.push_back('\r');
            break; case '0':  resolved_string.push_back('\0');
            break; case '\\': resolved_string.push_back('\\');
            break; case '\'': resolved_string.push_back('\'');
            break; case '"':  resolved_string.push_back('"'); 
            break; case '`':  resolved_string.push_back('`'); 
            break; case '$':  resolved_string.push_back(-1);  
            break; default:   throw_unrecognized_escape_sequence(
                token, 
                next_char
            );
        }
    }
    return resolved_string;
}

Expression Parser::parse_string_literal() {
    assert_token_is_string_literal(source_tokens, iterator);
    assert_string_literal_properly_formatted(iterator);
    std::string string_literal_value = resolve_escaped_characters(*iterator);
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
    std::string char_literal_value = resolve_escaped_characters(*iterator);
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