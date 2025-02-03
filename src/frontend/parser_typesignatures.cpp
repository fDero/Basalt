//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <assert.h>

#include "syntax/primitive_types.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "frontend/parser.hpp"
#include "language/expressions.hpp"

TypeSignature Parser::parse_simple_typesignature() {
    if (iterator->sourcetext == pointer_type_symbol) return parse_pointer_type();
    if (iterator->sourcetext == array_type_first_symbol) return parse_array_type();
    if (iterator->sourcetext == slice_type_symbol) return parse_slice_type();
    if (primitive_types.find(iterator->sourcetext) != primitive_types.end()) {
        return parse_primitive_type();
    }
    return (is_template_type(iterator->sourcetext))
        ? parse_template_type()
        : parse_custom_type();
}

TypeSignature Parser::parse_typesignature() {
    ensure_there_are_still_tokens(source_tokens, iterator);
    const Token& first_type_token = *iterator;
    TypeSignature type_signature = parse_simple_typesignature();
    return (iterator == source_tokens.end() || iterator->sourcetext != "|")
        ? type_signature
        : parse_inline_union(first_type_token, type_signature);
}

TypeSignature Parser::parse_inline_union(const Token& first_type_token, const TypeSignature& first_type) {
    std::vector<TypeSignature> alternatives { first_type };
    while (iterator != source_tokens.end() && iterator->sourcetext == "|") {
        assert_token_matches(source_tokens, iterator++, "|");
        alternatives.push_back(parse_typesignature());
    }
    return InlineUnion(first_type_token, alternatives);
}

bool Parser::is_template_type(const std::string& type_name) {
    if (template_generics_encountered_so_far.empty()) return false;
    auto template_type_search_outcome = std::find(template_generics_encountered_so_far.begin(), template_generics_encountered_so_far.end(), type_name);
    return template_type_search_outcome != template_generics_encountered_so_far.end();
}

TypeSignature Parser::parse_template_type() {
    return TemplateType { *(iterator++) };
}

TypeSignature Parser::parse_primitive_type() {
    return PrimitiveType { *(iterator++) };
}

TypeSignature Parser::parse_pointer_type() {
    const Token& pointer_type_symbol_token = *iterator;
    assert_token_matches(source_tokens, iterator++, pointer_type_symbol);
    return PointerType { pointer_type_symbol_token, parse_typesignature() };
}

TypeSignature Parser::parse_array_type() {
    const Token& array_square_bracket_token = *iterator;
    assert (array_type_first_symbol == "[");
    assert_token_matches(source_tokens, iterator++, array_type_first_symbol);
    ensure_token_is_fixed_array_length(source_tokens, iterator);
    const Token& array_length_token = *(iterator++);
    assert_token_matches(source_tokens, iterator++, "]");
    const TypeSignature array_stored_type = parse_typesignature();
    size_t array_length = std::stoull(array_length_token.sourcetext);
    return ArrayType { array_square_bracket_token, array_length, array_stored_type };
}

TypeSignature Parser::parse_slice_type() {
    const Token& slice_type_symbol_token = *iterator;
    assert_token_matches(source_tokens, iterator++, slice_type_symbol);
    const TypeSignature slice_stored_type = parse_typesignature();
    return SliceType { slice_type_symbol_token, slice_stored_type };
}

TypeSignature Parser::parse_custom_type() {
    std::string package_prefix = parse_package_prefix();
    ensure_token_is_typesignature(source_tokens, iterator);
    assert_type_is_properly_formatted(iterator);
    const Token& typesignature_token = *(iterator++);
    const std::vector<TypeSignature> generics = parse_concrete_generics();
    CustomType custom_type { typesignature_token, generics };
    custom_type.package_prefix = package_prefix;
    return custom_type;
}

std::vector<std::string> Parser::parse_template_generics() {
    template_generics_encountered_so_far.clear();
    if (iterator == source_tokens.end() || iterator->sourcetext != "<") return {};
    assert_token_matches(source_tokens, iterator, "<");
    const Token& angular_brackets_opening = *(iterator++);
    while (iterator != source_tokens.end() && iterator->sourcetext != ">") {
        ensure_token_is_simple_type_for_template_generics(iterator);
        template_generics_encountered_so_far.push_back(( iterator++ )->sourcetext);
        ensure_either_comma_or_closed_angular_for_generics(source_tokens, angular_brackets_opening, iterator);
        std::advance(iterator, iterator != source_tokens.end() && iterator->sourcetext == ",");
    }
    assert_token_matches(source_tokens, iterator++, ">");
    return template_generics_encountered_so_far;
}

std::vector<TypeSignature> Parser::parse_concrete_generics() {
    if (iterator == source_tokens.end() || iterator->sourcetext != "<") return {};
    assert_token_matches(source_tokens, iterator, "<");
    const Token& angular_brackets_opening = *(iterator++);
    std::vector<TypeSignature> concrete_generics;
    while (iterator != source_tokens.end() && iterator->sourcetext != ">") {
        concrete_generics.push_back(parse_typesignature());
        ensure_either_comma_or_closed_angular_for_generics(source_tokens, angular_brackets_opening, iterator);
        std::advance(iterator, iterator != source_tokens.end() && iterator->sourcetext == ",");
    }
    assert_token_matches(source_tokens, iterator++, ">");
    return concrete_generics;
}