//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <vector>

#include "errors/internal_errors.hpp"

void assert_token_matches(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator, 
    const std::string& text
) {
    #ifdef DEBUG_BUILD
    if (iterator == source_tokens.end()) {
        throw InternalError {
            "somehow the parser was expecting  a token but there was none, "
            "the last token in the tokens vector was: " + source_tokens.back().sourcetext
        };
    }
    if (iterator->sourcetext != text) {
        throw InternalError {
            "somehow the parser expected the token to be: " + text + ", "
            "while the token itself is: " + iterator->sourcetext
        };
    }
    #endif
}

void assert_token_is_of_given_type(
    const std::vector<Token> source_tokens,
    const std::vector<Token>::iterator& iterator, 
    const std::string& expected_type_str,
    const Token::Type expected_type
) {
    #ifdef DEBUG_BUILD
    if (iterator == source_tokens.end()) {
        throw InternalError {
            "somehow the parser was expecting  a token but there was none, "
            "the last token in the tokens vector was: " + source_tokens.back().sourcetext
        };
    }
    if (iterator->type != expected_type) {
        throw InternalError {
            "somehow the parser attempted to parse a " + expected_type_str + " while the current "
            "token has a clearly different type, the token itself is: " + iterator->sourcetext
        };
    }
    #endif
}

void assert_token_is_string_literal(const std::vector<Token>& source_tokens, const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    assert_token_is_of_given_type(source_tokens, iterator, "string literal", Token::Type::string_literal);
    #endif
}

void assert_token_is_boolean_literal(const std::vector<Token>& source_tokens, const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    assert_token_is_of_given_type(source_tokens, iterator, "boolean literal", Token::Type::boolean_literal);
    #endif
}

void assert_token_is_character_literal(const std::vector<Token>& source_tokens, const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    assert_token_is_of_given_type(source_tokens, iterator, "character literal", Token::Type::character_literal);
    #endif
}

void assert_token_is_floating_literal(const std::vector<Token>& source_tokens, const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    assert_token_is_of_given_type(source_tokens, iterator, "floating literal", Token::Type::floating_literal);
    #endif
}

void assert_token_is_integer_literal(const std::vector<Token>& source_tokens, const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    assert_token_is_of_given_type(source_tokens, iterator, "integer literal", Token::Type::integer_literal);
    #endif
}

void assert_token_is_text(const std::vector<Token>& source_tokens, const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    assert_token_is_of_given_type(source_tokens, iterator, "text", Token::Type::text);
    #endif
}