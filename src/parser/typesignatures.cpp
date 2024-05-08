
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"
#include <assert.h>

[[nodiscard]] TypeSignature Parser::parse_typesignature(){
    if (iterator->sourcetext == pointer_type_symbol) return parse_pointer_type();
    if (iterator->sourcetext == array_type_first_symbol) return parse_array_type();
    if (iterator->sourcetext == slice_type_symbol) return parse_slice_type();
    ensure_token_is_typesignature(source_tokens, iterator);
    TypeSignature typesignature = parse_base_type();
    return typesignature;
}

[[nodiscard]] TypeSignature Parser::parse_pointer_type(){
    const Token& pointer_type_symbol_token = *iterator;
    assert_token_matches(source_tokens, iterator++, pointer_type_symbol);
    return PointerType { pointer_type_symbol_token, parse_typesignature() };
}

[[nodiscard]] TypeSignature Parser::parse_array_type(){
    const Token& array_square_bracket_token = *iterator;
    assert (array_type_first_symbol == "[");
    assert_token_matches(source_tokens, iterator++, array_type_first_symbol);
    ensure_token_is_fixed_array_length(source_tokens, iterator);
    const Token& array_length_token = *(iterator++);
    assert_token_matches(source_tokens, iterator++, "]");
    const TypeSignature array_stored_type = parse_typesignature();
    int array_length = std::stoi(array_length_token.sourcetext);
    return ArrayType { array_square_bracket_token, array_length, array_stored_type };
}

[[nodiscard]] TypeSignature Parser::parse_slice_type(){
    const Token& slice_type_symbol_token = *iterator;
    assert_token_matches(source_tokens, iterator++, slice_type_symbol);
    const TypeSignature slice_stored_type = parse_typesignature();
    return SliceType { slice_type_symbol_token, slice_stored_type };
}

[[nodiscard]] TypeSignature Parser::parse_base_type(){
    std::string package_prefix = parse_package_prefix();
    assert_token_is_simple_type(source_tokens, iterator);
    assert_type_is_properly_formatted(iterator);
    const Token& typesignature_token = *(iterator++);
    const std::vector<TypeSignature> template_generics = parse_concrete_generics();
    BaseType base_type { package_prefix, typesignature_token, template_generics };
    return base_type;
}

[[nodiscard]] std::vector<std::string> Parser::parse_template_generics(){
    if (iterator == source_tokens.end() || iterator->sourcetext != "<") return {};
    assert_token_matches(source_tokens, iterator, "<");
    const Token& angular_brackets_opening = *(iterator++);
    std::vector<std::string> template_generics;
    while (iterator != source_tokens.end() && iterator->sourcetext != ">"){
        ensure_token_is_simple_type_for_template_generics(iterator);
        template_generics.push_back(( iterator++ )->sourcetext);
        ensure_either_comma_or_closed_angular_for_generics(source_tokens, angular_brackets_opening, iterator);
        std::advance(iterator, iterator != source_tokens.end() && iterator->sourcetext == ",");
    }
    assert_token_matches(source_tokens, iterator++, ">");
    return template_generics;
}

[[nodiscard]] std::vector<TypeSignature> Parser::parse_concrete_generics(){
    if (iterator == source_tokens.end() || iterator->sourcetext != "<") return {};
    assert_token_matches(source_tokens, iterator, "<");
    const Token& angular_brackets_opening = *(iterator++);
    std::vector<TypeSignature> concrete_generics;
    while (iterator != source_tokens.end() && iterator->sourcetext != ">"){
        concrete_generics.push_back(parse_typesignature());
        ensure_either_comma_or_closed_angular_for_generics(source_tokens, angular_brackets_opening, iterator);
        std::advance(iterator, iterator != source_tokens.end() && iterator->sourcetext == ",");
    }
    assert_token_matches(source_tokens, iterator++, ">");
    return concrete_generics;
}