//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/syntax.hpp"
#include "frontend/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] Expression Parser::compose_dot_member_access(const Expression& left_operand) {
    assert_token_matches(source_tokens, iterator, ".");
    const Token& operator_token = *(iterator++);
    assert_token_is_text(source_tokens, iterator);
    assert_identifier_is_properly_formatted(iterator);
    const std::string& member_name = iterator->sourcetext;
    std::advance(iterator, 1);
    DotMemberAccess dot_member_access { operator_token, left_operand, member_name };
    return rotate_to_match_dot_member_access_priority(dot_member_access);
}

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
    SquareBracketsAccess square_brackets_access { open_bracket_token, left_operand, index_expression };
    return rotate_to_match_square_brackets_access_priority(square_brackets_access);
}

[[nodiscard]] Expression Parser::compose_type_operator(const Expression& expression) {
    assert_token_is_type_operator(iterator);
    const Token& operator_token = *iterator;
    std::advance(iterator, 1);
    TypeSignature typesignature = parse_typesignature();
    TypeOperator is_operator { operator_token, expression, typesignature };
    return rotate_to_match_type_operator_priority(is_operator);
}