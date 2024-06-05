
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "toolchain/parser.hpp"
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
    std::string varname = ( iterator++ )->sourcetext;
    TypeSignature vartype = parse_type_after_object_declaration();
    std::optional<Expression> varvalue = parse_value_after_object_declaration();
    ensure_token_matches(source_tokens, iterator++, ";");
    return VariableDeclaration { varname, vartype, varvalue, var_token };
}

[[nodiscard]] Statement Parser::parse_constant_definition() {
    const Token& const_token = *iterator;
    assert_token_matches(source_tokens, iterator++, "const");
    ensure_token_is_identifier(source_tokens, iterator);
    std::string constname = ( iterator++ )->sourcetext;
    TypeSignature consttype = parse_type_after_object_declaration();
    assert_token_matches(source_tokens, iterator++, "=");
    Expression constvalue = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ";");
    return ConstDeclaration { constname, consttype, constvalue, const_token };
}