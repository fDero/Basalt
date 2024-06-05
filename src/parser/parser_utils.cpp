
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "toolchain/parser.hpp"
#include "language/definitions.hpp"
#include "language/expressions.hpp"

Parser::Parser(const std::vector<Token>& tokens) {
    source_tokens = tokens; 
    iterator = source_tokens.begin(); 
}

[[nodiscard]] Filerepresentation Parser::parse_everything() {
    Filerepresentation output;
    output.file_metadata.packagename = parse_package_name();
    parse_import_section(output);
    parse_alias_section(output);
    parse_source_code(output);
    return output;
}

void Parser::parse_source_code(Filerepresentation& output) {
    while (iterator != source_tokens.end()) {
        switch (iterator->type) {
            break; case Token::Type::func_keyword:    output.func_defs.push_back(parse_function_definition());
            break; case Token::Type::struct_keyword:  output.type_defs.push_back(parse_struct_definition());
            break; case Token::Type::union_keyword:   output.type_defs.push_back(parse_union_definition());
            break; default:                           throw_invalid_use_of_token_within_global_scope(iterator);
        }
    }
}

void Parser::parse_alias_section(Filerepresentation& output) {
    while (iterator != source_tokens.end() && iterator->type == Token::Type::alias_keyword) {
        ensure_there_are_still_tokens(source_tokens, std::next(iterator));
        switch(std::next(iterator)->type) {
            break; case Token::Type::type: output.type_defs.push_back(parse_type_alias()); 
            break; default: throw_invalid_use_of_token_within_global_scope(iterator);
        }
    }
}

void Parser::parse_import_section(Filerepresentation& output) {
    while (iterator != source_tokens.end() && iterator->type == Token::Type::import_keyword) {
        output.file_metadata.imports.push_back(parse_package_import());
    }
}