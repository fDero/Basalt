
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "toolchain/parser.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"

[[nodiscard]] std::optional<TypeSignature> Parser::parse_type_after_object_declaration(){
    if (iterator->sourcetext != ":") return std::nullopt;
    std::advance(iterator,1);
    return parse_typesignature();
}

[[nodiscard]] std::optional<Expression> Parser::parse_value_after_object_declaration(){
    if (iterator->sourcetext != "=") return std::nullopt;
    std::advance(iterator,1);
    return parse_expression();
}

[[nodiscard]] Statement Parser::parse_variable_definition(){
    assert_token_matches(source_tokens, iterator, "var");
    const Token& var_token = *(iterator++);
    ensure_token_is_identifier(source_tokens, iterator);
    std::string varname = ( iterator++ )->sourcetext;
    std::optional<TypeSignature> vartype = parse_type_after_object_declaration();
    std::optional<Expression> varvalue = parse_value_after_object_declaration();
    ensure_token_matches(source_tokens, iterator++, ";");
    //assert_variable_is_well_formed(varname, vartype, varvalue);
    return VariableDeclaration { varname, vartype, varvalue };
}

[[nodiscard]] Statement Parser::parse_constant_definition(){
    assert_token_matches(source_tokens, iterator, "const");
    const Token& var_token = *(iterator++);
    ensure_token_is_identifier(source_tokens, iterator);
    std::string varname = ( iterator++ )->sourcetext;
    std::optional<TypeSignature> vartype = parse_type_after_object_declaration();
    assert_token_matches(source_tokens, iterator++, "=");
    Expression varvalue = parse_expression();
    ensure_token_matches(source_tokens, iterator++, ";");
    //assert_constant_is_well_formed(varname, vartype, varvalue);
    return ConstDeclaration { varname, vartype, varvalue };
}