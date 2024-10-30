//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <vector>

#include "frontend/tokenizer.hpp"
#include "errors/parsing_errors.hpp"

void ensure_parenthesis_gets_closed_in_expression_wrapping(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const Token& parenthesis_opening_token
) {
    if (iterator == source_tokens.end() || iterator->sourcetext != ")") {
        throw ParsingError {
            "parenthesis got opened but never closed",
            parenthesis_opening_token
        };
    }
}

void ensure_square_bracket_gets_closed_in_expression_wrapping(
    const std::vector<Token>& source_tokens,
    const std::vector<Token>::iterator& iterator,
    const Token& brackets_opening_token
) {
    if (iterator == source_tokens.end() || iterator->sourcetext != "]") {
        throw ParsingError {
            "square brackets got opened but never closed",
            brackets_opening_token
        };
    }
}