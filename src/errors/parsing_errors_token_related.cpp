//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cctype>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/token.hpp"
#include "misc/debug_informations_aware_entity.hpp"



static DebugInformationsAwareEntity end_of_file_location(
    const std::vector<Token>& source_tokens
) {
    if (source_tokens.empty()) {
        return DebugInformationsAwareEntity("unknown", 0, 0, 0);
    }
    DebugInformationsAwareEntity location = source_tokens.back().as_debug_informations_aware_entity();
    location.char_pos += source_tokens.back().sourcetext.size();
    return location;
}

// the position right after the token preceding `iterator`
static std::optional<DebugInformationsAwareEntity> location_after_previous_token(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (source_tokens.empty() || iterator == source_tokens.begin()) {
        return std::nullopt;
    }
    const Token& previous = *std::prev(iterator);
    DebugInformationsAwareEntity location = previous.as_debug_informations_aware_entity();
    location.char_pos += previous.sourcetext.size();
    return location;
}

void ensure_there_are_still_tokens(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError(
            "unexpected end of file",
            end_of_file_location(source_tokens)
        );
    }
}

void ensure_either_comma_or_closed_paren_for_function_args(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError(
            "unexpected end of file inside an argument list: expected `,` or `)`",
            end_of_file_location(source_tokens)
        );
    }
    if (iterator->sourcetext != ")" && iterator->sourcetext != ",") {
        throw ParsingError(
            "expected `,` or `)` in the argument list, got " + backticked(iterator->sourcetext),
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
        throw ParsingError(
            "unexpected end of file inside a generics list: expected `,` or `>`",
            end_of_file_location(source_tokens),
            "the generics list was opened at " + describe_location(generics_opening_angular_bracket)
        );
    }
    if (iterator->sourcetext != ">" && iterator->sourcetext != ",") {
        throw ParsingError(
            "expected `,` or `>` in the generics list, got " + backticked(iterator->sourcetext),
            *iterator,
            "the generics list was opened at " + describe_location(generics_opening_angular_bracket)
        );
    }
}

void ensure_token_matches(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const std::string& text_to_match
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError(
            "expected " + backticked(text_to_match) + ", got end of file",
            end_of_file_location(source_tokens)
        );
    }
    if (iterator->sourcetext == text_to_match) {
        return;
    }
    std::optional<DebugInformationsAwareEntity> after_previous = location_after_previous_token(source_tokens, iterator);
    if (text_to_match == ";" && after_previous.has_value()) {
        throw ParsingError(
            "expected `;` after " + backticked(std::prev(iterator)->sourcetext),
            after_previous.value()
        );
    }
    throw ParsingError(
        "expected " + backticked(text_to_match) + ", got " + backticked(iterator->sourcetext),
        *iterator
    );
}

void ensure_token_is_identifier(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError(
            "expected a name, got end of file",
            end_of_file_location(source_tokens)
        );
    }
    if (!islower(iterator->sourcetext[0])) {
        throw ParsingError(
            "expected a name, got " + backticked(iterator->sourcetext),
            *iterator,
            "names of variables, constants, functions and fields start with a lowercase letter"
        );
    }
}

void ensure_token_is_struct_name(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError(
            "expected a type name, got end of file",
            end_of_file_location(source_tokens)
        );
    }
    if (!isupper(iterator->sourcetext[0])) {
        throw ParsingError(
            "expected a type name, got " + backticked(iterator->sourcetext),
            *iterator,
            "type names start with an uppercase letter"
        );
    }
}

void ensure_token_is_simple_type_for_template_generics(
    const std::vector<Token>::iterator& iterator
) {
    if (iterator->type != Token::Type::type) {
        throw ParsingError(
            "expected the name of a generic type parameter, got " + backticked(iterator->sourcetext),
            *iterator,
            "generic type parameters are plain names starting with an uppercase letter, "
            "and they cannot have generics themselves"
        );
    }
}

void ensure_token_is_fixed_array_length(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError(
            "expected the length of the array, got end of file",
            end_of_file_location(source_tokens)
        );
    }
    if (iterator->type != Token::Type::integer_literal) {
        throw ParsingError(
            "expected the length of the array, got " + backticked(iterator->sourcetext),
            *iterator,
            "the length of an array must be an integer literal, like `[3]Int`"
        );
    }
}

void ensure_token_is_typesignature(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator
) {
    if (iterator == source_tokens.end()) {
        throw ParsingError(
            "expected a type, got end of file",
            end_of_file_location(source_tokens)
        );
    }
    if (iterator->type != Token::Type::type) {
        throw ParsingError(
            "expected a type, got " + backticked(iterator->sourcetext),
            *iterator
        );
    }
}

void ensure_file_is_not_empty(
    const std::vector<Token>& source_tokens,
    const std::string& filename
) {
    if (!source_tokens.empty()) {
        return;
    }
    throw ParsingError(
        "the file is empty",
        DebugInformationsAwareEntity(filename, 1, 0, 0),
        "every Basalt file must start by declaring its package, like `package main;`"
    );
}
