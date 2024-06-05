
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] Expression Parser::parse_array_literal() {
    const Token& array_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "[");
    ensure_there_are_still_tokens(source_tokens, iterator);
    int array_length = -1;
    if (iterator->sourcetext != "]") {
        ensure_token_is_fixed_array_length(source_tokens, iterator);
        array_length = std::stoi(iterator->sourcetext);
        iterator++;
    }
    assert_token_matches(source_tokens, iterator++, "]");
    TypeSignature stored_type = parse_typesignature();
    assert_token_matches(source_tokens, iterator++, "{");
    ensure_there_are_still_tokens(source_tokens, iterator);
    std::vector<Expression> elements;
    if (iterator->sourcetext != "}") {
        elements.push_back(parse_expression());
    }
    while (iterator->sourcetext == ",") {
        ++iterator;
        elements.push_back(parse_expression());
    }
    assert_token_matches(source_tokens, iterator++, "}");
    return ArrayLiteral { array_length, stored_type, elements, array_token };
}

[[nodiscard]] Expression Parser::parse_expression_wrapped_in_square_brackets() {
    assert_token_matches(source_tokens, iterator, "[");
    const Token& open_bracket_token = *(iterator++);
    Expression expression = parse_expression();
    expression.wrap_in_parenthesis();
    ensure_square_bracket_gets_closed_in_expression_wrapping(source_tokens, iterator++, open_bracket_token);
    return expression;
}

[[nodiscard]] Expression Parser::parse_expression_wrapped_in_parenthesis() {
    assert_token_matches(source_tokens, iterator, "(");
    const Token& open_paren_token = *(iterator++);
    Expression expression = parse_expression();
    expression.wrap_in_parenthesis();
    ensure_parenthesis_gets_closed_in_expression_wrapping(source_tokens, iterator++, open_paren_token);
    return expression;
}

[[nodiscard]] Expression Parser::parse_terminal_expression() {
    ensure_there_are_still_tokens(source_tokens, iterator);
    switch(iterator->type) {
        break; case Token::Type::text: {
            if (std::next(iterator) == source_tokens.end() || std::next(iterator)->sourcetext != "(") { 
                return parse_identifier();
            }
            FunctionCall fcall = parse_function_call();
            return fcall;
        }
        break; case Token::Type::symbol: {
            if (iterator->sourcetext == "(") return parse_expression_wrapped_in_parenthesis(); 
            else if (iterator->sourcetext == "[") return parse_array_literal();
            else return parse_prefix_operator();
        }
        break; case Token::Type::integer_literal:   return parse_integer_literal();
        break; case Token::Type::floating_literal:  return parse_floating_literal();
        break; case Token::Type::boolean_literal:   return parse_boolean_literal();
        break; case Token::Type::character_literal: return parse_character_literal();
        break; case Token::Type::string_literal:    return parse_string_literal();
        break; default:                             throw_expression_expected_got_unrecognized(iterator);
    }
}

[[nodiscard]] Expression Parser::parse_prefix_operator() {
    assert_token_is_prefix_operator(iterator);
    const Token& operator_token = *iterator;
    std::advance(iterator, 1);
    Expression operand = parse_terminal_expression();
    return UnaryOperator { operator_token, operand };
}

[[nodiscard]] Expression Parser::parse_expression() {
    Expression expression = parse_terminal_expression();
    while(!expression_ended()) {
        if (is_binary_operator()) expression = compose_binary_operator(expression);
        else if (is_square_bracket()) expression = compose_square_bracket_access(expression);
        else if (is_type_operator()) expression = compose_type_operator(expression);
        else throw_expression_expected_got_unrecognized(iterator);
    }
    return expression;
}

[[nodiscard]] bool Parser::is_type_operator() {
    return (
        iterator->type == Token::Type::as_keyword || 
        iterator->type == Token::Type::is_keyword 
    );
}

[[nodiscard]] bool Parser::is_binary_operator() {
    if (iterator == source_tokens.end()) return false;
    auto binary_operator_search_outcome =  infix_operators_priority.find(iterator->sourcetext);
    auto not_found = infix_operators_priority.end();
    return (binary_operator_search_outcome != not_found);
}

[[nodiscard]] bool Parser::is_square_bracket() {
    if (iterator == source_tokens.end()) return false;
    return (iterator->sourcetext == "[");
}

[[nodiscard]] bool Parser::expression_ended() {
    if (iterator == source_tokens.end()) return true;
    if (iterator->sourcetext == ")")     return true;
    if (iterator->sourcetext == "]")     return true;
    if (iterator->sourcetext == "}")     return true;
    if (iterator->sourcetext == ";")     return true;
    if (iterator->sourcetext == ",")     return true;
    if (iterator->sourcetext == "=")     return true;
    return false;
}