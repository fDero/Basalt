//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>
#include <vector>

#include "errors/compilation_error.hpp"
#include "errors/diagnostic_formatting.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/token.hpp"
#include "language/definitions.hpp"
#include "language/typesignatures.hpp"



void throw_expression_expected_got_unrecognized(
    const std::vector<Token>::iterator& iterator
) {
    throw ParsingError(
        "expected an expression, got " + backticked(iterator->sourcetext),
        *iterator
    );
}

void throw_invalid_use_of_token_within_global_scope(
    const std::vector<Token>::iterator& iterator
) {
    throw ParsingError(
        backticked(iterator->sourcetext) + " cannot appear at the top level of a file",
        *iterator,
        "only `func`, `extern`, `struct`, `union` and `alias` definitions are allowed here"
    );
}

void ensure_union_has_multiple_alternatives(
    const TypeSignature& parsed_alternatives,
    const UnionDefinition& union_definition
) {
    if (parsed_alternatives.is<InlineUnion>()) {
        return;
    }
    throw ParsingError(
        "union " + backticked(union_definition.def_name) + " must have at least two alternatives, "
        "but only " + backticked(parsed_alternatives.to_source_string()) + " was given",
        union_definition,
        "list the alternatives separated by `|`, for example: `union Number = Int | Float;`"
    );
}

void ensure_character_literal_has_exactly_one_character(
    const Token& token,
    const std::string& char_literal_value
) {
    if (char_literal_value.size() == 1) {
        return;
    }
    throw TokenizationError(
        "character literal " + token.sourcetext + " must contain exactly one character, but it contains " +
        std::to_string(char_literal_value.size()),
        token,
        "use double quotes for strings: \"...\""
    );
}

void throw_unrecognized_escape_sequence(
    const Token& token,
    char current_char
) {
    throw TokenizationError(
        "unrecognized escape sequence " + backticked(std::string{'\\', current_char}) + " in " + token.sourcetext,
        token,
        "supported escape sequences are: \\n \\t \\r \\0 \\\\ \\' \\\" \\` \\$"
    );
}
