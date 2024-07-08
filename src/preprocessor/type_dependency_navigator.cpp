
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TypeDependencyNavigator::TypeDependencyNavigator(
    TypeDefinitionsRegister& program_representation
) : program_representation(program_representation) {}

void TypeDependencyNavigator::verify_that_the_type_exists(const TypeSignature& type_signature) {
    if (type_signature.is<PointerType>()) {
        verify_that_the_type_exists(type_signature.get<PointerType>().pointed_type);
    }
    else if (type_signature.is<ArrayType>()) {
        verify_that_the_type_exists(type_signature.get<ArrayType>().stored_type);
    }
    else if (type_signature.is<SliceType>()) {
        verify_that_the_type_exists(type_signature.get<SliceType>().stored_type);
    }
    else if (type_signature.is<InlineUnion>()) {
        for (const TypeSignature& alternative : type_signature.get<InlineUnion>().alternatives){
            verify_that_the_type_exists(alternative);
        }
    }
    else if (type_signature.is<CustomType>()) {
        const CustomType& custom_type = type_signature.get<CustomType>();
        std::ignore = program_representation.retrieve_type_definition(custom_type);
    }
}

void TypeDependencyNavigator::visit_typesignature(const TypeSignature& typesignature, const std::vector<std::string>& generics) {
    if (typesignature.is_generic()) {
        return;
    }
    else if (typesignature.is<PointerType>()) {
        verify_that_the_type_exists(typesignature.get<PointerType>().pointed_type);
    }
    else if (typesignature.is<ArrayType>()) {
        visit_typesignature(typesignature.get<ArrayType>().stored_type, generics);
    }
    else if (typesignature.is<SliceType>()) {
        verify_that_the_type_exists(typesignature.get<SliceType>().stored_type);
    }
    else if (typesignature.is<InlineUnion>()){
        for (const TypeSignature& alternative : typesignature.get<InlineUnion>().alternatives){
            visit_typesignature(alternative, generics);
        }
    }
    else if (typesignature.is<CustomType>()) {
        const CustomType& custom_type = typesignature.get<CustomType>();
        TypeDefinition type_definition = program_representation.retrieve_type_definition(custom_type);
        visit_type_definition(typesignature, type_definition, generics);
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
