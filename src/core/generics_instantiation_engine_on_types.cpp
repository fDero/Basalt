//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/generics_substitution_rules.hpp"
#include "core/generics_instantiation_engine.hpp"
#include "language/typesignatures.hpp"
#include "errors/internal_errors.hpp"

TypeSignature GenericsInstantiationEngine::instantiate_generic_typesignature(const TypeSignature& type_signature) {
    switch (type_signature.typesiganture_kind()) {
        case TypeSignatureBody::Kind::template_type:  return instantiate_template_type(type_signature.get<TemplateType>());
        case TypeSignatureBody::Kind::custom_type:    return instantiate_custom_type(type_signature.get<CustomType>());
        case TypeSignatureBody::Kind::pointer_type:   return instantiate_pointer_type(type_signature.get<PointerType>());
        case TypeSignatureBody::Kind::array_type:     return instantiate_array_type(type_signature.get<ArrayType>());
        case TypeSignatureBody::Kind::slice_type:     return instantiate_slice_type(type_signature.get<SliceType>());
        case TypeSignatureBody::Kind::inline_union:   return instantiate_inline_union(type_signature.get<InlineUnion>());    
        case TypeSignatureBody::Kind::primitive_type: return type_signature;
    }
    assert_unreachable();
}

TypeSignature GenericsInstantiationEngine::instantiate_template_type(const TemplateType& template_type) {
    for (const GenericSubstitutionRule& rule : rules) {
        if (rule.to_be_replaced == template_type.type_name) {
            return rule.replacement;
        }
    }
    return template_type;
}

CustomType GenericsInstantiationEngine::instantiate_custom_type(const CustomType& type_signature) {
    CustomType custom_type = type_signature;
    for (TypeSignature& type_parameter : custom_type.type_parameters) {
        type_parameter = instantiate_generic_typesignature(type_parameter);
    }
    return custom_type;
}

PointerType GenericsInstantiationEngine::instantiate_pointer_type(const PointerType& type_signature) {
    PointerType pointer_type = type_signature;
    pointer_type.pointed_type = instantiate_generic_typesignature(pointer_type.pointed_type);
    return pointer_type;
}

ArrayType GenericsInstantiationEngine::instantiate_array_type(const ArrayType& type_signature) {
    ArrayType array_type = type_signature;
    array_type.stored_type = instantiate_generic_typesignature(array_type.stored_type);
    return array_type;
}

SliceType GenericsInstantiationEngine::instantiate_slice_type(const SliceType& type_signature) {
    SliceType array_type = type_signature;
    array_type.stored_type = instantiate_generic_typesignature(array_type.stored_type);
    return array_type;
}

InlineUnion GenericsInstantiationEngine::instantiate_inline_union(const InlineUnion& type_signature) {
    InlineUnion inline_union = type_signature;
    for (TypeSignature& alternative : inline_union.alternatives) {
        alternative = instantiate_generic_typesignature(alternative);
    }
    return inline_union;    
}