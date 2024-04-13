
#include "toolchain/generics.hpp"
#include "errors/internal_errors.hpp"
#include <regex>
#include <optional>
#include <iostream>

template <typename TypesignatureKind>
inline bool typesignatures_are(const TypeSignature& type1, const TypeSignature& type2){
    return (type1.is<TypesignatureKind>() && type2.is<TypesignatureKind>());
}

std::optional<GenericSubstitutionRuleSet> apply_generics_to_array_type(
    const TypeSignature& template_type,
    const TypeSignature& concrete_type,
    const std::vector<std::string>& generic_names
){
    assert_typesignature_is<ArrayType>(template_type);
    assert_typesignature_is<ArrayType>(concrete_type);
    int template_array_len = template_type.get<ArrayType>().array_length;
    int concrete_array_len = concrete_type.get<ArrayType>().array_length;
    if (template_array_len != concrete_array_len) return std::nullopt;
    return apply_generics_to_typesignature(
        template_type.get<ArrayType>().stored_type,
        concrete_type.get<ArrayType>().stored_type,
        generic_names
    );
}

std::optional<GenericSubstitutionRuleSet> apply_generics_to_slice_type(
    const TypeSignature& template_type,
    const TypeSignature& concrete_type,
    const std::vector<std::string>& generic_names
){
    assert_typesignature_is<SliceType>(template_type);
    assert_typesignature_is<SliceType>(concrete_type);
    return apply_generics_to_typesignature(
        template_type.get<SliceType>().stored_type,
        concrete_type.get<SliceType>().stored_type,
        generic_names
    );
}

std::optional<GenericSubstitutionRuleSet> apply_generics_to_pointer_type(
    const TypeSignature& template_type,
    const TypeSignature& concrete_type,
    const std::vector<std::string>& generic_names
){
    assert_typesignature_is<PointerType>(template_type);
    assert_typesignature_is<PointerType>(concrete_type);
    return apply_generics_to_typesignature(
        template_type.get<PointerType>().pointed_type,
        concrete_type.get<PointerType>().pointed_type, 
        generic_names
    );
}

std::optional<GenericSubstitutionRuleSet> apply_generics_to_typesignature(
    const TypeSignature& template_type,
    const TypeSignature& concrete_type,
    const std::vector<std::string>& generic_names
){
    if (typesignatures_are<ArrayType>(template_type, concrete_type))
        return apply_generics_to_array_type(template_type, concrete_type, generic_names);
    if (typesignatures_are<SliceType>(template_type, concrete_type))
        return apply_generics_to_slice_type(template_type, concrete_type, generic_names);
    if (typesignatures_are<PointerType>(template_type, concrete_type))
        return apply_generics_to_pointer_type(template_type, concrete_type, generic_names);
    if (template_type.is<BaseType>())
        return apply_generics_to_base_type(template_type, concrete_type, generic_names);
    return std::nullopt;
}

