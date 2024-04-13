
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] StructDefinition::Field Parser::parse_struct_field(){
    assert_token_is_text(source_tokens, iterator);
    std::string field_name = iterator->sourcetext;
    std::advance(iterator, 1);
    ensure_token_matches(source_tokens, iterator++, ":");
    TypeSignature field_type = parse_typesignature();
    ensure_token_matches(source_tokens, iterator++, ";");
    return StructDefinition::Field {field_name, field_type};
}

[[nodiscard]] StructDefinition Parser::parse_struct_definition(){
    assert_token_matches(source_tokens, iterator++, "struct");
    ensure_token_is_struct_name(source_tokens, iterator);
    StructDefinition struct_def(*iterator);
    std::advance(iterator, 1);
    struct_def.template_generics_names = parse_template_generics();
    ensure_token_matches(source_tokens, iterator++, "{");
    while (iterator != source_tokens.end() && iterator->sourcetext != "}"){
        ensure_token_is_identifier(source_tokens, iterator);
        StructDefinition::Field field = parse_struct_field();
        struct_def.fields.push_back(field);
    }
    ensure_token_matches(source_tokens, iterator++, "}");
    struct_def.generate_pattern_tag_name();
    return struct_def;
}