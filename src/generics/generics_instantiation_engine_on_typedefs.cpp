
#include "language/generics.hpp"
#include "language/typesystem.hpp"
#include "errors/internal_errors.hpp"

StructDefinition GenericsInstantiationEngine::instantiate_generic_struct(const StructDefinition& struct_definition) const {
    StructDefinition struct_def = struct_definition;
    struct_def.template_generics_names.clear();
    for (StructDefinition::Field& field : struct_def.fields) {
        field.field_type = instantiate_generic_typesignature(field.field_type);
    }
    return struct_def;
}

UnionDefinition GenericsInstantiationEngine::instantiate_generic_union(const UnionDefinition& union_definition) const {
    UnionDefinition union_def = union_definition;
    union_def.template_generics_names.clear();
    for (TypeSignature& type : union_def.types) {
        type = instantiate_generic_typesignature(type);
    }
    return union_def;
}

TypeAlias GenericsInstantiationEngine::instantiate_generic_alias(const TypeAlias& type_alias) const {
    TypeAlias alias = type_alias;
    alias.template_generics_names.clear();
    alias.aliased_type = instantiate_generic_typesignature(alias.aliased_type);
    return alias;
}

TypeDefinition GenericsInstantiationEngine::instantiate_generic_typedefinition(const TypeDefinition& type_definition) const {
    if (type_definition.is<StructDefinition>()) {
        return instantiate_generic_struct(type_definition.get<StructDefinition>());
    }
    if (type_definition.is<UnionDefinition>()) {
        return instantiate_generic_union(type_definition.get<UnionDefinition>());
    }
    if (type_definition.is<TypeAlias>()) {
        return instantiate_generic_alias(type_definition.get<TypeAlias>());
    }
    assert_unreachable();
}