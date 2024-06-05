
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] Expression Parser::compose_binary_operator(const Expression& left_operand) {
    assert_token_is_binary_operator(iterator);
    const Token& operator_token = *(iterator++);
    Expression right_operand = parse_terminal_expression();
    BinaryOperator binary_operator{ operator_token, left_operand, right_operand };
    return rotate_binary_operator_to_match_operators_priority(binary_operator);
}

[[nodiscard]] Expression Parser::compose_square_bracket_access(const Expression& left_operand) {
    assert_token_matches(source_tokens, iterator, "[");
    const Token& open_bracket_token = *iterator;
    Expression index_expression = parse_expression_wrapped_in_square_brackets();
    BinaryOperator binary_operator{ open_bracket_token, left_operand, index_expression };
    binary_operator.operator_text = square_brackets_access;
    return rotate_binary_operator_to_match_operators_priority(binary_operator);
}

[[nodiscard]] Expression Parser::compose_type_operator(const Expression& expression) {
    assert_token_is_type_operator(iterator);
    const Token& operator_token = *iterator;
    std::advance(iterator, 1);
    TypeSignature typesignature = parse_typesignature();
    TypeOperator is_operator { operator_token, expression, typesignature };
    return rotate_to_match_is_operator_priority(is_operator);
}