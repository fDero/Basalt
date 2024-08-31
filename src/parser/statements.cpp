
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/parser.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

[[nodiscard]] Statement Parser::parse_non_keyword_initialized_statements() {
    Expression expression = parse_expression();
    if (expression.is<FunctionCall>()) {
        FunctionCall function_call = expression.get<FunctionCall>();
        ensure_token_matches(source_tokens, iterator++, ";");
        return function_call;
    }
    ensure_there_are_still_tokens(source_tokens, iterator);
    const Token& assignment_token = *iterator;
    ensure_token_matches(source_tokens, iterator++, "=");
    Expression right_hand_side = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ";");
    return Assignment { expression, right_hand_side, assignment_token };
}

[[nodiscard]] Statement Parser::parse_statement() {
    ensure_there_are_still_tokens(source_tokens, iterator);
    switch (iterator->type) {
        break; case Token::Type::if_keyword:       return parse_if_statement();
        break; case Token::Type::while_keyword:    return parse_while_loop();
        break; case Token::Type::until_keyword:    return parse_until_loop();
        break; case Token::Type::break_keyword:    return parse_break_keyword();
        break; case Token::Type::continue_keyword: return parse_continue_keyword();
        break; case Token::Type::return_keyword:   return parse_return_keyword();
        break; case Token::Type::var_keyword:      return parse_variable_definition();
        break; case Token::Type::const_keyword:    return parse_constant_definition();
        break; default:                            return parse_non_keyword_initialized_statements();
    }
}

[[nodiscard]] std::vector<Statement> Parser::parse_multiline_code_block() {
    assert_token_matches(source_tokens, iterator++, "{");
    std::vector<Statement> code;
    while (iterator != source_tokens.end() && iterator->sourcetext != "}") {
        code.push_back(parse_statement());
    }
    ensure_there_are_still_tokens(source_tokens, iterator);
    ensure_token_matches(source_tokens, iterator++, "}");
    return code;
}

[[nodiscard]] std::vector<Statement> Parser::parse_code_block() {
    assert_tokens_not_ended(iterator, source_tokens);
    return (iterator->sourcetext == "{")
        ? parse_multiline_code_block() 
        : std::vector<Statement>{ parse_statement() };
}