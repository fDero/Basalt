
#include "language/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>

CustomType::CustomType(
    const Token& typename_token, 
    const std::vector<TypeSignature>& generics
) 
    : TypeSignatureBody(typename_token) 
    , type_name(typename_token.sourcetext)
    , type_parameters(generics)
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