
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

TypeDependencyNavigator::TypeDependencyNavigator(
    TypeDefinitionsRegister& types_register
) : types_register(types_register) {
    visited_definitions.reserve(types_register.struct_definitions.size());
}

void TypeDependencyNavigator::verify_that_the_type_exists(const TypeSignature& type_signature){
    if (type_signature.is<PointerType>()) {
        verify_that_the_type_exists(type_signature.get<PointerType>().pointed_type);
    }
    else if (type_signature.is<ArrayType>()) {
        verify_that_the_type_exists(type_signature.get<ArrayType>().stored_type);
    }
    else if (type_signature.is<SliceType>()) {
        verify_that_the_type_exists(type_signature.get<SliceType>().stored_type);
    }
    else if (primitive_types.find(type_signature.get<BaseType>().type_name) == primitive_types.end()) {
        std::ignore = types_register.retrieve(type_signature);
    }
}

void TypeDependencyNavigator::visit_typesignature(const TypeSignature& typesignature){
    if (typesignature.is<PointerType>()) {
        verify_that_the_type_exists(typesignature.get<PointerType>().pointed_type);
    }
    else if (typesignature.is<ArrayType>()) {
        visit_typesignature(typesignature.get<ArrayType>().stored_type);
    }
    else if (typesignature.is<SliceType>()) {
        verify_that_the_type_exists(typesignature.get<SliceType>().stored_type);
    }
    else if (!typesignature.is_primitive_type()) {
        TypeDefinition type_definition = types_register.retrieve(typesignature);
        if (type_definition.is<StructDefinition>()) {
            const StructDefinition& struct_def = type_definition.get<StructDefinition>();
            instantiation_and_visit_struct(struct_def, typesignature);
        }
        else {
            const UnionDefinition& union_def = type_definition.get<UnionDefinition>();
            instantiation_and_visit_union(union_def, typesignature);
        }
    }
}