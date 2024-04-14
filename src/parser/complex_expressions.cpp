
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] Expression Parser::parse_expression_wrapped_in_square_brackets(){
    assert_token_matches(source_tokens, iterator, "[");
    const Token& open_bracket_token = *(iterator++);
    Expression expression = parse_expression();
    expression.wrap_in_parenthesis();
    ensure_square_bracket_gets_closed_in_expression_wrapping(source_tokens, iterator++, open_bracket_token);
    return expression;
}

[[nodiscard]] Expression Parser::parse_expression_wrapped_in_parenthesis(){
    assert_token_matches(source_tokens, iterator, "(");
    const Token& open_paren_token = *(iterator++);
    Expression expression = parse_expression();
    expression.wrap_in_parenthesis();
    ensure_parenthesis_gets_closed_in_expression_wrapping(source_tokens, iterator++, open_paren_token);
    return expression;
}

[[nodiscard]] Expression Parser::parse_terminal_expression(){
    ensure_there_are_still_tokens(source_tokens, iterator);
    switch(iterator->type){
        break; case Token::Type::text: {
            if (std::next(iterator) == source_tokens.end() || std::next(iterator)->sourcetext != "("){ 
                return parse_identifier();
            }
            FunctionCall fcall = parse_function_call();
            return fcall;
        }
        break; case Token::Type::symbol: {
            return (iterator->sourcetext == "(")? 
                parse_expression_wrapped_in_parenthesis() : parse_prefix_operator();
        }
        break; case Token::Type::integer_literal:   return parse_integer_literal();
        break; case Token::Type::floating_literal:  return parse_floating_literal();
        break; case Token::Type::boolean_literal:   return parse_boolean_literal();
        break; case Token::Type::character_literal: return parse_character_literal();
        break; case Token::Type::string_literal:    return parse_string_literal();
        break; default: throw_expression_expected_got_unrecognized(iterator);
    }
}

[[nodiscard]] Expression Parser::parse_prefix_operator(){
    assert_token_is_prefix_operator(iterator);
    std::string operator_text = ( iterator++ )->sourcetext;
    Expression operand = parse_terminal_expression();
    return UnaryOperator { operator_text, operand };
}

[[nodiscard]] Expression Parser::parse_expression(){
    Expression expression = parse_terminal_expression();
    while(!expression_ended()) {
        if (is_binary_operator()) expression = compose_binary_operator(expression);
        else if (is_square_bracket()) expression = compose_square_bracket_access(expression);
        else throw_expression_expected_got_unrecognized(iterator);
    }
    return expression;
}