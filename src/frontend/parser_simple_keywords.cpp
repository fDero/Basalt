//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/parser.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

Statement Parser::parse_break_keyword() {
    const Token& break_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "break");
    ensure_token_matches(source_tokens, iterator++, ";");
    return Break { break_token };
}

Statement Parser::parse_continue_keyword() {
    const Token& continue_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "continue");
    ensure_token_matches(source_tokens, iterator++, ";");
    return Continue { continue_token };
}

Statement Parser::parse_return_keyword() {
    const Token& return_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "return");
    ensure_there_are_still_tokens(source_tokens, iterator);
    if (iterator->sourcetext == ";") {
        std::advance(iterator, 1);
        return Return { std::nullopt, return_token };
    }
    Expression return_expression = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ";");
    return Return { return_expression, return_token };
}