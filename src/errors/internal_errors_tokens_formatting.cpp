//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cctype>
#include <string>
#include <vector>

#include "errors/internal_errors.hpp"
#include "frontend/token.hpp"
#include "syntax/infixes.hpp"
#include "syntax/prefixes.hpp"


void assert_integer_literal_properly_formatted(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    for (const char digit : iterator->sourcetext) {
        if (!isdigit(digit)) {
            throw_internal_error("somehow the tokenizer assigned the integer literal type to a non integer literal token, "
                "the token itself is: " + iterator->sourcetext);
        }
    }
    #endif
}

void assert_floating_literal_properly_formatted(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    short floating_points = 0;
    bool only_digits_and_points = true;
    for (const char digit : iterator->sourcetext) {
        floating_points += (digit == '.');
        only_digits_and_points &= (isdigit(digit) || digit == '.');
    }
    if (!only_digits_and_points || floating_points != 1) {
        throw_internal_error(
            "somehow the tokenizer assigned the floating literal type to a non floating literal token, "
            "the token itself is: " + iterator->sourcetext
        );
    }
    #endif
}

void assert_character_literal_properly_formatted(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    const std::string& txt = iterator->sourcetext;
    if (txt.size() < 3 || txt.front() != txt.back()) {
        throw_internal_error("somehow the tokenizer assigned the character literal type to a non character literal token, "
            "the token itself is: " + iterator->sourcetext);
    }
    #endif
}

void assert_boolean_literal_properly_formatted(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    if (iterator->sourcetext != "true" && iterator->sourcetext != "false") {
        throw_internal_error("somehow the tokenizer assigned the boolean literal type to a non boolean literal token, "
            "the token itself is: " + iterator->sourcetext);
    }
    #endif
}

void assert_string_literal_properly_formatted(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    if (iterator->sourcetext[0] != iterator->sourcetext.back()) {
        throw_internal_error("somehow the tokenizer assigned the string literal type to a non string literal token, "
            "the token itself is: " + iterator->sourcetext);
    }
    #endif
}

void assert_identifier_is_properly_formatted(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    if (!islower(iterator->sourcetext[0])) {
        throw_internal_error("somehow the tokenizer assigned the text type to a non text token, "
            "the token itself is: " + iterator->sourcetext);
    }
    #endif
}

void assert_type_is_properly_formatted(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    if (!isupper(iterator->sourcetext[0])) {
        throw_internal_error("somehow the tokenizer tought this token was a base-type-signature but it clearly isn't, "
            "the token itself is: " + iterator->sourcetext);
    }
    #endif
}

void assert_token_is_prefix_operator(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    if (prefix_operators_priority.find(iterator->sourcetext) == prefix_operators_priority.end()) {
        throw_internal_error("somehow the parser expected this token to be a prefix operator, instead it is: " +
            iterator->sourcetext);
    }
    #endif
}

void assert_token_is_binary_operator(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    if (infix_operators_priority.find(iterator->sourcetext) == infix_operators_priority.end()) {
        throw_internal_error("somehow the parser expected this token to be a binary operator, instead it is: " +
            iterator->sourcetext);
    }
    #endif
}

void assert_token_is_type_operator(const std::vector<Token>::iterator& iterator) {
    #ifdef DEBUG_BUILD
    if (type_operators.find(iterator->sourcetext) == type_operators.end()) {
        throw_internal_error("somehow the parser expected this token to be a type operator, instead it is: " +
            iterator->sourcetext);
    }
    #endif
}

void assert_tokens_not_ended(const std::vector<Token>::iterator& iterator, const std::vector<Token>& tokens) {
    #ifdef DEBUG_BUILD
    if (iterator == tokens.end()) {
        throw_internal_error("somehow the parser expected the token stream to be not yet ended, while instead every token already got processed");
    }
    #endif
}