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
#include "syntax/keywords.hpp"

Statement Parser::parse_if_statement() {
    const Token& if_token = *iterator;
    assert_token_matches(source_tokens, iterator++, if_keyword);
    ensure_token_matches(source_tokens, iterator++, "(");
    Expression condition = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ")");
    std::vector<Statement> then_branch = parse_code_block();
    if (iterator == source_tokens.end() || iterator->sourcetext != else_keyword) {
        return Conditional {condition, then_branch, {}, if_token};
    }
    assert_token_matches(source_tokens, iterator++, else_keyword);
    std::vector<Statement> else_branch = parse_code_block();
    return Conditional {condition, then_branch, else_branch, if_token};
}

Statement Parser::parse_while_loop() {
    const Token& while_token = *iterator;
    assert_token_matches(source_tokens, iterator++, while_keyword);
    ensure_token_matches(source_tokens, iterator++, "(");
    Expression condition = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ")");
    std::vector<Statement> loop_body = parse_code_block();
    return WhileLoop {condition, loop_body, while_token};
}

Statement Parser::parse_until_loop() {
    const Token& until_token = *iterator;
    assert_token_matches(source_tokens, iterator++, until_keyword);
    ensure_token_matches(source_tokens, iterator++, "(");
    Expression condition = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ")");
    std::vector<Statement> loop_body = parse_code_block();
    return UntilLoop {condition, loop_body, until_token};
}