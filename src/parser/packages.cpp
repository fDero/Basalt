
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "toolchain/parser.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

[[nodiscard]] std::string Parser::parse_package_name(){
    assert_token_matches(source_tokens, iterator++, "package");
    std::string package_name = iterator->sourcetext;
    std::advance(iterator, 1);
    ensure_token_matches(source_tokens, iterator++, ";");
    return package_name;
}

[[nodiscard]] std::string Parser::parse_package_import(){
    assert_token_matches(source_tokens, iterator++, "import");
    std::string package_name = iterator->sourcetext;
    std::advance(iterator, 1);
    ensure_token_matches(source_tokens, iterator++, ";");
    return package_name;
}

[[nodiscard]] TypeAlias Parser::parse_type_alias(){
    assert_token_matches(source_tokens, iterator++, "alias");
    const Token& alias_token = *iterator;
    ensure_token_is_typesignature(source_tokens, iterator++);
    std::vector<std::string> template_generics = parse_template_generics();
    ensure_token_matches(source_tokens, iterator++, "=");
    TypeSignature alias_type = parse_typesignature();
    ensure_token_matches(source_tokens, iterator++, ";");
    return TypeAlias(alias_token, template_generics, alias_type);
}