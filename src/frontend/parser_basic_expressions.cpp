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

Expression Parser::parse_string_literal() {
    assert_token_is_string_literal(source_tokens, iterator);
    assert_string_literal_properly_formatted(iterator);
    StringLiteral string_literal = StringLiteral(*iterator);
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
    CharLiteral char_literal = CharLiteral(*iterator);
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