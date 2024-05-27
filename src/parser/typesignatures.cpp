
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
    if (primitive_types.find(iterator->sourcetext) != primitive_types.end()) {
        return parse_primitive_type();
    }
    if (is_template_type(iterator->sourcetext)) {
        return parse_template_type();
    }
    else {
        return parse_custom_type();
    }
}

[[nodiscard]] bool Parser::is_template_type(const std::string& type_name){
    if (template_generics.empty()) return false;
    auto template_type_search_outcome = std::find(template_generics.begin(), template_generics.end(), type_name);
    return template_type_search_outcome != template_generics.end();
}

[[nodiscard]] TypeSignature Parser::parse_template_type(){
    return TemplateType { *(iterator++) };
}

[[nodiscard]] TypeSignature Parser::parse_primitive_type(){
    return PrimitiveType { *(iterator++) };
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

[[nodiscard]] TypeSignature Parser::parse_custom_type(){
    PackageName package_prefix = parse_package_prefix();
    ensure_token_is_typesignature(source_tokens, iterator);
    assert_type_is_properly_formatted(iterator);
    const Token& typesignature_token = *(iterator++);
    const ConcreteGenerics generics = parse_concrete_generics();
    CustomType custom_type { typesignature_token, generics };
    custom_type.package_prefix = package_prefix;
    return custom_type;
}

[[nodiscard]] std::vector<std::string> Parser::parse_template_generics(){
    template_generics.clear();
    if (iterator == source_tokens.end() || iterator->sourcetext != "<") return {};
    assert_token_matches(source_tokens, iterator, "<");
    const Token& angular_brackets_opening = *(iterator++);
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
    ConcreteGenerics concrete_generics;
    while (iterator != source_tokens.end() && iterator->sourcetext != ">"){
        concrete_generics.push_back(parse_typesignature());
        ensure_either_comma_or_closed_angular_for_generics(source_tokens, angular_brackets_opening, iterator);
        std::advance(iterator, iterator != source_tokens.end() && iterator->sourcetext == ",");
    }
    assert_token_matches(source_tokens, iterator++, ">");
    return concrete_generics;
}