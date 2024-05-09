
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

[[nodiscard]] std::string Parser::parse_package_prefix(){
    std::string package_prefix = "";
    while(iterator != source_tokens.end() && std::next(iterator) != source_tokens.end()){
        if (std::next(iterator)->sourcetext == namespace_concatenation){
            package_prefix += iterator->sourcetext + namespace_concatenation;
            std::advance(iterator, 2);
            continue;
        }
        break;
    }
    return package_prefix;
}