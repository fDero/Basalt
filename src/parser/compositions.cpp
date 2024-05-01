
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] Expression Parser::compose_binary_operator(const Expression& left_operand){
    assert_token_is_binary_operator(iterator);
    const Token& operator_token = *(iterator++);
    Expression right_operand = parse_terminal_expression();
    BinaryOperator binary_operator{ operator_token.sourcetext, left_operand, right_operand };
    return rotate_binary_operator_to_match_operators_priority(binary_operator);
}

[[nodiscard]] Expression Parser::compose_square_bracket_access(const Expression& left_operand){
    assert_token_matches(source_tokens, iterator, "[");
    Expression index_expression = parse_expression_wrapped_in_square_brackets();
    BinaryOperator binary_operator{ square_brackets_access, left_operand, index_expression };
    return rotate_binary_operator_to_match_operators_priority(binary_operator);
}