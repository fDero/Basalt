
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "toolchain/parser.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

[[nodiscard]] Statement Parser::parse_non_keyword_initialized_statements(){
    Expression expression = parse_expression();
    if (expression.is<FunctionCall>()) {
        FunctionCall fcall = expression.get<FunctionCall>();
        ensure_token_matches(source_tokens, iterator++, ";");
        return fcall;
    }
    ensure_there_are_still_tokens(source_tokens, iterator);
    const Token& assignment_token = *iterator;
    ensure_token_matches(source_tokens, iterator++, "=");
    Expression right_hand_side = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ";");
    return Assignment { expression, right_hand_side, assignment_token };
}

[[nodiscard]] Statement Parser::parse_statement(){
    ensure_there_are_still_tokens(source_tokens, iterator);
    if (iterator->sourcetext == "if")       return parse_if_statement();
    if (iterator->sourcetext == "while")    return parse_while_loop();
    if (iterator->sourcetext == "until")    return parse_until_loop();
    if (iterator->sourcetext == "break")    return parse_break_keyword();
    if (iterator->sourcetext == "continue") return parse_continue_keyword();
    if (iterator->sourcetext == "return")   return parse_return_keyword();
    if (iterator->sourcetext == "var")      return parse_variable_definition();
    if (iterator->sourcetext == "const")    return parse_constant_definition();
    return parse_non_keyword_initialized_statements();
}

[[nodiscard]] std::vector<Statement> Parser::parse_multiline_code_block(){
    assert_token_matches(source_tokens, iterator++, "{");
    std::vector<Statement> code;
    while (iterator != source_tokens.end() && iterator->sourcetext != "}"){
        code.push_back(parse_statement());
    }
    ensure_there_are_still_tokens(source_tokens, iterator);
    ensure_token_matches(source_tokens, iterator++, "}");
    return code;
}

[[nodiscard]] std::vector<Statement> Parser::parse_code_block(){
    assert_tokens_not_ended(iterator, source_tokens);
    return (iterator->sourcetext == "{")
        ? parse_multiline_code_block() 
        : std::vector<Statement>{ parse_statement() };
}