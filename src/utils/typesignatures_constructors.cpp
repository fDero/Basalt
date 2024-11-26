//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/generics_substitution_rules.hpp"
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

InlineUnion::InlineUnion(const DebugInformationsAwareEntity &first_type_token, const std::vector<TypeSignature> &alternatives)
    : TypeSignatureBody(first_type_token)
    , alternatives(alternatives)
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

PrimitiveType::PrimitiveType(
    const std::string& type_name,
    const DebugInformationsAwareEntity& debug_info
) 
    : TypeSignatureBody(debug_info) 
    , type_name(type_name) 
{}

PointerType::PointerType(
    const DebugInformationsAwareEntity& pointer_symbol_token,
    const TypeSignature& pointed
) 
    : TypeSignatureBody(pointer_symbol_token) 
    , pointed_type(pointed) {}

ArrayType::ArrayType(
    const DebugInformationsAwareEntity& array_open_square_bracket_token,
    size_t length, 
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