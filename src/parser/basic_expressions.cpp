
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

[[nodiscard]] Expression Parser::parse_string_literal(){
    assert_token_is_string_literal(source_tokens, iterator);
    assert_string_literal_properly_fromatted(iterator);
    std::string string_literal_text = ( iterator++ )->sourcetext;
    return StringLiteral { string_literal_text };
}

[[nodiscard]] Expression Parser::parse_boolean_literal(){
    assert_token_is_boolean_literal(source_tokens, iterator);
    assert_boolean_literal_properly_formatted(iterator);
    std::string boolean_literal_text = ( iterator++ )->sourcetext;
    bool boolean_literal_value = (boolean_literal_text == boolean_true_literal);
    return BoolLiteral { boolean_literal_value };
}

[[nodiscard]] Expression Parser::parse_character_literal(){
    assert_token_is_character_literal(source_tokens, iterator);
    assert_character_literal_properly_formatted(iterator);
    std::string character_literal_text = ( iterator++ )->sourcetext;
    char character_literal_value = character_literal_text[1];
    return CharLiteral { character_literal_value };
}

[[nodiscard]] Expression Parser::parse_floating_literal(){
    assert_token_is_floating_literal(source_tokens, iterator);
    assert_floating_literal_properly_formatted(iterator);
    std::string floating_literal_text = ( iterator++ )->sourcetext;
    double floating_literal_value = std::stod(floating_literal_text);
    return FloatLiteral { floating_literal_value };
}

[[nodiscard]] Expression Parser::parse_integer_literal(){
    assert_token_is_integer_literal(source_tokens, iterator);
    assert_integer_literal_properly_formatted(iterator);
    std::string integer_literal_text = ( iterator++ )->sourcetext;
    int integer_literal_value = std::stoi(integer_literal_text);
    return IntLiteral { integer_literal_value };
}

[[nodiscard]] Expression Parser::parse_identifier(){
    assert_token_is_text(source_tokens, iterator);
    assert_identifier_is_properly_fromatted(iterator);
    std::string string_literal_text = ( iterator++ )->sourcetext;
    return Identifier { string_literal_text };
}