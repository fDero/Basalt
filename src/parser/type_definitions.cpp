//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/syntax.hpp"
#include "frontend/parser.hpp"
#include "language/expressions.hpp"

#include "errors/preprocessing_errors.hpp" // <-- ensure_typesignature_is

[[nodiscard]] StructDefinition::Field Parser::parse_struct_field() {
    assert_token_is_text(source_tokens, iterator);
    std::string field_name = iterator->sourcetext;
    std::advance(iterator, 1);
    ensure_token_matches(source_tokens, iterator++, ":");
    TypeSignature field_type = parse_typesignature();
    ensure_token_matches(source_tokens, iterator++, ";");
    return StructDefinition::Field {field_name, field_type};
}

[[nodiscard]] StructDefinition Parser::parse_struct_definition() {
    assert_token_matches(source_tokens, iterator++, "struct");
    ensure_token_is_struct_name(source_tokens, iterator);
    StructDefinition struct_def(*iterator);
    std::advance(iterator, 1);
    struct_def.template_generics_names = parse_template_generics();
    ensure_token_matches(source_tokens, iterator++, "{");
    while (iterator != source_tokens.end() && iterator->sourcetext != "}") {
        ensure_token_is_identifier(source_tokens, iterator);
        StructDefinition::Field field = parse_struct_field();
        struct_def.fields.push_back(field);
    }
    ensure_token_matches(source_tokens, iterator++, "}");
    return struct_def;
}

[[nodiscard]] UnionDefinition Parser::parse_union_definition() {
    assert_token_matches(source_tokens, iterator++, "union");
    ensure_token_is_struct_name(source_tokens, iterator);
    UnionDefinition union_def(*iterator);
    std::advance(iterator, 1);
    union_def.template_generics_names = parse_template_generics();
    ensure_token_matches(source_tokens, iterator++, "=");
    TypeSignature inline_union_ts = parse_typesignature();
    ensure_typesignature_is<InlineUnion>(inline_union_ts);
    union_def.types = std::move(inline_union_ts.get<InlineUnion>().alternatives);
    ensure_token_matches(source_tokens, iterator++, ";");
    return union_def;
}

[[nodiscard]] TypeAlias Parser::parse_type_alias() {
    assert_token_matches(source_tokens, iterator++, "alias");
    const Token& alias_token = *iterator;
    ensure_token_is_typesignature(source_tokens, iterator++);
    std::vector<std::string> template_generics = parse_template_generics();
    ensure_token_matches(source_tokens, iterator++, "=");
    TypeSignature alias_type = parse_typesignature();
    ensure_token_matches(source_tokens, iterator++, ";");
    TypeAlias type_alias(alias_token, template_generics, alias_type);
    return type_alias;
}