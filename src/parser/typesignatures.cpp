
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "toolchain/parser.hpp"
#include "language/expressions.hpp"

[[nodiscard]] TypeSignature Parser::parse_typesignature(){
    if (iterator->sourcetext == "#") return parse_pointer_type();
    if (iterator->sourcetext == "[") return parse_array_type();
    if (iterator->sourcetext == "$") return parse_slice_type();
    ensure_token_is_typesignature(source_tokens, iterator);
    TypeSignature typesignature = parse_base_type();
    return typesignature;
}

[[nodiscard]] TypeSignature Parser::parse_pointer_type(){
    assert_token_matches(source_tokens, iterator++, "#");
    return PointerType { parse_typesignature() };
}

[[nodiscard]] TypeSignature Parser::parse_array_type(){
    assert_token_matches(source_tokens, iterator++, "[");
    ensure_token_is_fixed_array_length(source_tokens, iterator);
    const Token& array_length_token = *(iterator++);
    assert_token_matches(source_tokens, iterator++, "]");
    const TypeSignature array_stored_type = parse_typesignature();
    int array_length = std::stoi(array_length_token.sourcetext);
    return ArrayType { array_length, array_stored_type };
}

[[nodiscard]] TypeSignature Parser::parse_slice_type(){
    assert_token_matches(source_tokens, iterator++, "$");
    const TypeSignature slice_stored_type = parse_typesignature();
    return SliceType { slice_stored_type };
}

[[nodiscard]] TypeSignature Parser::parse_base_type(){
    assert_token_is_simple_type(source_tokens, iterator);
    assert_type_is_properly_formatted(iterator);
    const Token& typesignature_token = *(iterator++);
    std::string type_name = typesignature_token.sourcetext;
    const std::vector<TypeSignature> template_generics = parse_concrete_generics();
    BaseType base_type { type_name, template_generics };
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