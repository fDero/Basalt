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

[[nodiscard]] TypeSignature Parser::parse_type_after_object_declaration() {
    ensure_token_matches(source_tokens, iterator++, ":");
    return parse_typesignature();
}

[[nodiscard]] std::optional<Expression> Parser::parse_value_after_object_declaration() {
    if (iterator->sourcetext != "=") return std::nullopt;
    std::advance(iterator,1);
    return parse_expression();
}

[[nodiscard]] Statement Parser::parse_variable_definition() {
    const Token& var_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "var");
    ensure_token_is_identifier(source_tokens, iterator);
    std::string var_name = ( iterator++ )->sourcetext;
    TypeSignature var_type = parse_type_after_object_declaration();
    std::optional<Expression> var_value = parse_value_after_object_declaration();
    ensure_token_matches(source_tokens, iterator++, ";");
    return VariableDeclaration { var_name, var_type, var_value, var_token };
}

[[nodiscard]] Statement Parser::parse_constant_definition() {
    const Token& const_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "const");
    ensure_token_is_identifier(source_tokens, iterator);
    std::string const_name = ( iterator++ )->sourcetext;
    TypeSignature const_type = parse_type_after_object_declaration();
    assert_token_matches(source_tokens, iterator++, "=");
    Expression const_value = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ";");
    return ConstDeclaration { const_name, const_type, const_value, const_token };
}