
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/parser.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"

[[nodiscard]] Statement Parser::parse_if_statement() {
    const Token& if_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "if");
    ensure_token_matches(source_tokens, iterator++, "(");
    Expression condition = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ")");
    std::vector<Statement> then_brench = parse_code_block();
    if (iterator == source_tokens.end() || iterator->sourcetext != "else") {
        return Conditional {condition, then_brench, {}, if_token};
    }
    assert_token_matches(source_tokens, iterator++, "else");
    std::vector<Statement> else_branch = parse_code_block();
    return Conditional {condition, then_brench, else_branch, if_token};
}

[[nodiscard]] Statement Parser::parse_while_loop() {
    const Token& while_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "while");
    ensure_token_matches(source_tokens, iterator++, "(");
    Expression condition = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ")");
    std::vector<Statement> loop_body = parse_code_block();
    return WhileLoop {condition, loop_body, while_token};
}

[[nodiscard]] Statement Parser::parse_until_loop() {
    const Token& until_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "until");
    ensure_token_matches(source_tokens, iterator++, "(");
    Expression condition = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ")");
    std::vector<Statement> loop_body = parse_code_block();
    return UntilLoop {condition, loop_body, until_token};
}