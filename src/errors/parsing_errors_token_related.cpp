//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/parsing_errors.hpp"

std::optional<Token> extract_last_token(const std::vector<Token>& source_tokens) {
    return source_tokens.empty()
        ? std::nullopt 
        : std::optional<Token>(source_tokens.back());
}

void ensure_there_are_still_tokens(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError {
            "an expression was expected, instead got END-OF-FILE",
            extract_last_token(source_tokens)
        };
    }
}

void ensure_either_comma_or_closed_paren_for_function_args(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError {
            "arguments list to a function opened but never closed, "
            "another argument was expected, instead got END-OF-FILE",
            *iterator
        };
    }
    if (iterator->sourcetext != ")" && iterator->sourcetext != ",") {
        throw ParsingError {
            "arguments list to a function opened but never closed, "
            "a comma or a closed round parenthesis was expected",
            *iterator
        };
    }
}

void ensure_either_comma_or_closed_angular_for_generics(
    const std::vector<Token>& source_tokens,
    const Token& generics_opening_angular_bracket,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError {
            "arguments list to a function opened but never closed, "
            "another generic was expected, instead got END-OF-FILE",
            generics_opening_angular_bracket
        };
    }
    if (iterator->sourcetext != ">" && iterator->sourcetext != ",") {
        throw ParsingError {
            "generics list opened but never closed, "
            "a comma or a closed angular bracket was expected",
            generics_opening_angular_bracket
        };
    } 
}

void ensure_token_matches(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const std::string& text_to_match
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError {
            text_to_match + " was expected, expected, instead got END-OF-FILE", 
            extract_last_token(source_tokens)
        };
    }
    if (iterator->sourcetext != text_to_match) {
        throw ParsingError {
            text_to_match + " was expected, instead got: " + iterator->sourcetext,
            *iterator
        };
    } 
}

void ensure_token_is_identifier(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError {
            "an identifier was expected, instead got END-OF-FILE", extract_last_token(source_tokens)
        };
    }
    if (!islower(iterator->sourcetext[0])) {
        throw ParsingError {
           "an identifier was expected, instead got: " + iterator->sourcetext , *iterator
        };
    } 
}

void ensure_token_is_struct_name(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError {
            "an identifier was expected, instead got END-OF-FILE", extract_last_token(source_tokens)
        };
    }
    if (!isupper(iterator->sourcetext[0])) {
        throw ParsingError {
           "an identifier was expected, instead got: " + iterator->sourcetext , *iterator
        };
    } 
}

void ensure_token_is_simple_type_for_template_generics(
    const std::vector<Token>::iterator& iterator
) {
    if (iterator->type != Token::Type::type) {
        throw ParsingError {
            "unexpected token, a type was expected, instead " + iterator->sourcetext + " was given\n"
            "(remember that template generics cannot have generics themselves)",
            *iterator
        };
    }
}

void ensure_token_is_fixed_array_length(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError {
            "the length of an array was expected, instead got END-OF-FILE",
            extract_last_token(source_tokens)
        };
    }
    if (iterator->type != Token::Type::integer_literal) {
        throw ParsingError {
            "the length of an array was expected, instead " + iterator->sourcetext + " was given, "
            "please keep in mind that the size of an array should be a LITERAL not an complex expression",
            *iterator
        };
    }
}

void ensure_token_is_typesignature(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError {
            "a type was expected, instead got END-OF-FILE",
            extract_last_token(source_tokens)
        };
    }
    if (iterator->type != Token::Type::type) {
        throw ParsingError {
            "a type was expected, instead " + iterator->sourcetext + " was given",
            *iterator
        };
    }
}