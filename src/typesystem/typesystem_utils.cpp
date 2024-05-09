
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>

[[nodiscard]] std::string TypeSignature::to_string() const {
    return ptr->to_string();
}

[[nodiscard]] std::string TypeSignature::to_match_string() const { 
    return ptr->to_match_string(); 
}

[[nodiscard]] bool TypeSignature::is_generic(const std::vector<std::string>& generic_names) const { 
    return ptr->is_generic(generic_names); 
}

[[nodiscard]] bool TypeSignature::is_primitive_type() const { 
    return primitive_types.find(ptr->to_string()) != primitive_types.end();
}


BaseType::BaseType(
    const Token& typename_token, 
    const std::vector<TypeSignature>& generics
) 
    : TypeSignatureBody(typename_token) 
    , type_name(typename_token.sourcetext)
    , instantiationd_generics(generics)
{}

TemplateType::TemplateType(
    const Token& typename_token
) 
    : TypeSignatureBody(typename_token) 
    , type_name(typename_token.sourcetext) 
{}

PrimitiveType::PrimitiveType(
    const Token& typename_token
) 
    : TypeSignatureBody(typename_token) 
    , type_name(typename_token.sourcetext) 
{}

PointerType::PointerType(
    const Token& pointer_symbol_token,
    const TypeSignature& pointed
) 
    : TypeSignatureBody(pointer_symbol_token) 
    , pointed_type(pointed) {}

ArrayType::ArrayType(
    const Token& array_open_square_bracket_token,
    int length, 
    const TypeSignature& stored
) 
    : TypeSignatureBody(array_open_square_bracket_token) 
    , array_length(length)
    , stored_type(stored) 
{}

SliceType::SliceType(
    const Token& slice_type_symbol_token,
    const TypeSignature& stored
) 
    : TypeSignatureBody(slice_type_symbol_token) 
    , stored_type(stored) 
{}