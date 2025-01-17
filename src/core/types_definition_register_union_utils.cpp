//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/type_definitions_register.hpp"
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/generics_substitution_rules.hpp"

bool TypeDefinitionsRegister::is_union(
    const TypeSignature& maybe_union_type
) {
    return !fetch_union_alternatives(maybe_union_type).empty();
}

std::vector<TypeSignature> TypeDefinitionsRegister::fetch_union_alternatives(
    const TypeSignature& maybe_union_type
) {
    const TypeSignature& unaliased_maybe_union_type = unalias_type(maybe_union_type);
    if (unaliased_maybe_union_type.is<CustomType>()) {
        CustomType custom_type = unaliased_maybe_union_type.get<CustomType>();
        TypeDefinition type_definition = retrieve_type_definition(custom_type);
        return (type_definition.is<UnionDefinition>())
            ? type_definition.get<UnionDefinition>().types
            : std::vector<TypeSignature>();
    }
    return (unaliased_maybe_union_type.is<InlineUnion>())
        ? unaliased_maybe_union_type.get<InlineUnion>().alternatives
        : std::vector<TypeSignature>();
}

std::vector<TypeSignature> TypeDefinitionsRegister::fetch_non_union_compatible_types_for_union_assignment(
    const TypeSignature& maybe_union_type
) {
    std::vector<TypeSignature> alternatives = fetch_union_alternatives(maybe_union_type);
    return fetch_non_union_compatible_types_for_union_assignment(alternatives);
}

std::vector<TypeSignature> TypeDefinitionsRegister::fetch_non_union_compatible_types_for_union_assignment(
    const UnionDefinition& union_definition
) {
    return fetch_non_union_compatible_types_for_union_assignment(union_definition.types);
}

std::vector<TypeSignature> TypeDefinitionsRegister::fetch_non_union_compatible_types_for_union_assignment(
    const std::vector<TypeSignature>& alternatives
) {
    std::vector<TypeSignature> compatible_types;
    for (const TypeSignature& alternative : alternatives) {
        std::vector<TypeSignature> sub_alternatives = fetch_union_alternatives(alternative);
        if (sub_alternatives.empty()) {
            compatible_types.push_back(alternative);
            continue;
        } 
        std::vector<TypeSignature> sub_compatible_types = fetch_non_union_compatible_types_for_union_assignment(alternative);
        compatible_types.insert(compatible_types.end(), sub_compatible_types.begin(), sub_compatible_types.end());
    }
    return compatible_types;
}