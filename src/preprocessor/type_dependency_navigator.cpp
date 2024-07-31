
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

#include <iostream>

TypeDependencyNavigator::TypeDependencyNavigator(
    TypeDefinitionsRegister& program_representation
) : program_representation(program_representation) {}

void TypeDependencyNavigator::visit_type_definition(
    const TypeDefinition& type_definition
) {
    if (type_definition.is<StructDefinition>()) {
        visit_struct_definition(type_definition.get<StructDefinition>());
    }
    else if (type_definition.is<UnionDefinition>()) {
        visit_union_definition(type_definition.get<UnionDefinition>());
    }
    else if (type_definition.is<TypeAlias>()) {
        visit_typesignature(type_definition.get<TypeAlias>().aliased_type, {});
    }
}


void TypeDependencyNavigator::verify_that_the_type_exists(const TypeSignature& type_signature) {
    switch (type_signature.typesiganture_kind()) {
        break; case TypeSignatureBody::Kind::pointer_type:   verify_that_the_type_exists(type_signature.get<PointerType>().pointed_type);
        break; case TypeSignatureBody::Kind::array_type:     verify_that_the_type_exists(type_signature.get<ArrayType>().stored_type);
        break; case TypeSignatureBody::Kind::slice_type:     verify_that_the_type_exists(type_signature.get<SliceType>().stored_type);
        break; case TypeSignatureBody::Kind::custom_type:    std::ignore = program_representation.retrieve_type_definition(type_signature.get<CustomType>());
        break; case TypeSignatureBody::Kind::template_type:  return;
        break; case TypeSignatureBody::Kind::primitive_type: return;
        break; case TypeSignatureBody::Kind::inline_union: {    
            for (const TypeSignature& alternative : type_signature.get<InlineUnion>().alternatives){
                verify_that_the_type_exists(alternative);
            }
        }
    }
}

void TypeDependencyNavigator::visit_typesignature(const TypeSignature& typesignature, const std::vector<std::string>& generics) {
    if (typesignature.is_generic()) {
        return;
    }
    switch (typesignature.typesiganture_kind()) {
        break; case TypeSignatureBody::Kind::template_type:  return;
        break; case TypeSignatureBody::Kind::primitive_type: return;
        break; case TypeSignatureBody::Kind::pointer_type:   verify_that_the_type_exists(typesignature.get<PointerType>().pointed_type);
        break; case TypeSignatureBody::Kind::array_type:     visit_typesignature(typesignature.get<ArrayType>().stored_type, generics);
        break; case TypeSignatureBody::Kind::slice_type:     verify_that_the_type_exists(typesignature.get<SliceType>().stored_type);
        break; case TypeSignatureBody::Kind::inline_union: {
            for (const TypeSignature& alternative : typesignature.get<InlineUnion>().alternatives){
                visit_typesignature(alternative, generics);
            }
        }
        break; case TypeSignatureBody::Kind::custom_type: {
            const CustomType& custom_type = typesignature.get<CustomType>();
            TypeDefinition type_definition = program_representation.retrieve_type_definition(custom_type);
            visit_type_definition(typesignature, type_definition, generics);
        }
    }
}

void TypeDependencyNavigator::visit_type_definition(
    const TypeSignature& typesignature,
    const TypeDefinition& type_definition,
    const std::vector<std::string>& generics
) {
    if (type_definition.is<StructDefinition>()) {
        const StructDefinition& struct_def = type_definition.get<StructDefinition>();
        visit_struct_definition(struct_def);
    }
    else if (type_definition.is<UnionDefinition>()) {
        const UnionDefinition& union_def = type_definition.get<UnionDefinition>();
        visit_union_definition(union_def);
    }
    else if (type_definition.is<TypeAlias>()) {
        const TypeAlias& alias_def = type_definition.get<TypeAlias>();
        visit_typesignature(alias_def.aliased_type, generics);
    }
}

void TypeDependencyNavigator::visit_union_definition(const UnionDefinition& union_definition) {
    std::string enum_id = union_definition.generate_union_id();
    ensure_type_not_already_visited_hence_no_cyclic_dependency(enum_id, visited_definitions);
    visited_definitions.insert(enum_id);
    for (const TypeSignature& alternative : union_definition.types) {
        visit_typesignature( alternative, union_definition.template_generics_names );
    }
}

void TypeDependencyNavigator::visit_struct_definition(const StructDefinition& struct_definition) {
    std::string struct_id = struct_definition.generate_struct_id();
    ensure_type_not_already_visited_hence_no_cyclic_dependency(struct_id, visited_definitions);
    visited_definitions.insert(struct_id);
    for (const StructDefinition::Field& field : struct_definition.fields) {
        visit_typesignature( field.field_type, struct_definition.template_generics_names );
    }
    visited_definitions.erase(struct_id);
}
