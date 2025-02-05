//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/parsing_errors.hpp"
using CompilationError::Kind::ParsingError;

static DebugInformationsAwareEntity extract_last_token(
    const std::vector<Token>& source_tokens
) {
    return source_tokens.empty()
        ? DebugInformationsAwareEntity("unknown", 0, 0, 0)
        : source_tokens.back().as_debug_informations_aware_entity();
}

void ensure_there_are_still_tokens(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        CompilationError::raise<ParsingError>(
            "an expression was expected, instead got END-OF-FILE",
            extract_last_token(source_tokens)
        );
    }
}

void ensure_either_comma_or_closed_paren_for_function_args(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        CompilationError::raise<ParsingError>(
            "arguments list to a function opened but never closed, "
            "another argument was expected, instead got END-OF-FILE",
            *iterator
        );
    }
    if (iterator->sourcetext != ")" && iterator->sourcetext != ",") {
        CompilationError::raise<ParsingError>(
            "arguments list to a function opened but never closed, "
            "a comma or a closed round parenthesis was expected",
            *iterator
        );
    }
}

void ensure_either_comma_or_closed_angular_for_generics(
    const std::vector<Token>& source_tokens,
    const Token& generics_opening_angular_bracket,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        CompilationError::raise<ParsingError>(
            "arguments list to a function opened but never closed, "
            "another generic was expected, instead got END-OF-FILE",
            generics_opening_angular_bracket
        );
    }
    if (iterator->sourcetext != ">" && iterator->sourcetext != ",") {
        CompilationError::raise<ParsingError>(
            "generics list opened but never closed, "
            "a comma or a closed angular bracket was expected",
            generics_opening_angular_bracket
        );
    } 
}

void ensure_token_matches(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const std::string& text_to_match
) {
    if (iterator == source_tokens.end()) {
        CompilationError::raise<ParsingError>(
            text_to_match + " was expected, expected, instead got END-OF-FILE", 
            extract_last_token(source_tokens)
        );
    }
    if (iterator->sourcetext != text_to_match) {
        CompilationError::raise<ParsingError>(
            text_to_match + " was expected, instead got: " + iterator->sourcetext,
            *iterator
        );
    } 
}

void ensure_token_is_identifier(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        CompilationError::raise<ParsingError>(
            "an identifier was expected, instead got END-OF-FILE", extract_last_token(source_tokens)
        );
    }
    if (!islower(iterator->sourcetext[0])) {
        CompilationError::raise<ParsingError>(
           "an identifier was expected, instead got: " + iterator->sourcetext , *iterator
        );
    } 
}

void ensure_token_is_struct_name(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        CompilationError::raise<ParsingError>(
            "an identifier was expected, instead got END-OF-FILE", extract_last_token(source_tokens)
        );
    }
    if (!isupper(iterator->sourcetext[0])) {
        CompilationError::raise<ParsingError>(
           "an identifier was expected, instead got: " + iterator->sourcetext , *iterator
        );
    } 
}

void ensure_token_is_simple_type_for_template_generics(
    const std::vector<Token>::iterator& iterator
) {
    if (iterator->type != Token::Type::type) {
        CompilationError::raise<ParsingError>(
            "unexpected token, a type was expected, instead " + iterator->sourcetext + " was given\n"
            "(remember that template generics cannot have generics themselves)",
            *iterator
        );
    }
}

void ensure_token_is_fixed_array_length(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        CompilationError::raise<ParsingError>(
            "the length of an array was expected, instead got END-OF-FILE",
            extract_last_token(source_tokens)
        );
    }
    if (iterator->type != Token::Type::integer_literal) {
        CompilationError::raise<ParsingError>(
            "the length of an array was expected, instead " + iterator->sourcetext + " was given, "
            "please keep in mind that the size of an array should be a LITERAL not an complex expression",
            *iterator
        );
    }
}

void ensure_token_is_typesignature(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        CompilationError::raise<ParsingError>(
            "a type was expected, instead got END-OF-FILE",
            extract_last_token(source_tokens)
        );
    }
    if (iterator->type != Token::Type::type) {
        CompilationError::raise<ParsingError>(
            "a type was expected, instead " + iterator->sourcetext + " was given",
            *iterator
        );
    }
}