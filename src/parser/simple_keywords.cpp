
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "toolchain/parser.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

[[nodiscard]] Statement Parser::parse_break_keyword(){
    assert_token_matches(source_tokens, iterator++, "break");
    ensure_token_matches(source_tokens, iterator++, ";");
    return Break {};
}

[[nodiscard]] Statement Parser::parse_continue_keyword(){
    assert_token_matches(source_tokens, iterator++, "continue");
    ensure_token_matches(source_tokens, iterator++, ";");
    return Continue {};
}

[[nodiscard]] Statement Parser::parse_return_keyword(){
    assert_token_matches(source_tokens, iterator++, "return");
    ensure_there_are_still_tokens(source_tokens, iterator);
    if (iterator->sourcetext == ";") {
        std::advance(iterator, 1);
        return Return { std::nullopt };
    }
    Expression return_expression = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ";");
    return Return { return_expression };
}