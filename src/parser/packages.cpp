
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "toolchain/parser.hpp"
#include "language/statements.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"
#include "language/functions.hpp"

[[nodiscard]] std::string Parser::parse_package_name() {
    assert_token_matches(source_tokens, iterator++, "package");
    std::string package_name = iterator->sourcetext;
    std::advance(iterator, 1);
    ensure_token_matches(source_tokens, iterator++, ";");
    return package_name;
}

[[nodiscard]] std::string Parser::parse_package_import() {
    assert_token_matches(source_tokens, iterator++, "import");
    std::string package_name = iterator->sourcetext;
    std::advance(iterator, 1);
    ensure_token_matches(source_tokens, iterator++, ";");
    return package_name;
}

[[nodiscard]] std::string Parser::parse_package_prefix() {
    if (std::next(iterator)->sourcetext != namespace_concatenation) {
        return "";
    }
    const std::string& package_prefix = iterator->sourcetext;
    std::advance(iterator, 2);
    return package_prefix;
}