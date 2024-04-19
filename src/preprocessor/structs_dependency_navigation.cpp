
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

void PreProcessor::preprocess_type_defintions(){
    for (const auto& struct_def_data : types_register.struct_definitions){
        TypeDependencyNavigator struct_nav(types_register);
        if (struct_def_data.second.is<StructDefinition>()){
            struct_nav.visit_struct_definition(struct_def_data.second.get<StructDefinition>());
        }
        else {
            throw std::runtime_error("UNEXPECTED TYPE (maybe union ? ?): " + struct_def_data.first);
        }
    }
}

TypeDependencyNavigator::TypeDependencyNavigator(
    TypeDefinitionsRegister& types_register
) : types_register(types_register) {
    visited_structs.reserve(types_register.struct_definitions.size());
}

void TypeDependencyNavigator::visit_struct_definition(const StructDefinition& struct_definition){
    std::string struct_id = struct_definition.generate_struct_id();
    ensure_struct_not_already_visited_hence_no_cyclic_dependency(struct_id, visited_structs);
    visited_structs.insert(struct_id);
    for (const StructDefinition::Field& field : struct_definition.fields){
        visit_struct_field( field, struct_definition.template_generics_names );
    }
}

void TypeDependencyNavigator::visit_struct_field(
    const StructDefinition::Field& field, 
    const std::vector<std::string>& struct_def_generics
){
    if (field.field_type.is_generic(struct_def_generics)) {
        return;
    }
    else if (field.field_type.is_core_language_type()) {
        verify_that_the_type_exists(field.field_type);
    }
    else {
        TypeDefinition field_def = types_register.retrieve(field.field_type);
        if (field_def.is<StructDefinition>()){
            StructDefinition& field_struct_def = field_def.get<StructDefinition>();
            if (!field_struct_def.template_generics_names.empty()){
                assert_typesignature_is<CustomType>(field.field_type);
                field_struct_def.instanciate_generics(field.field_type.get<CustomType>());
                types_register.store(field_struct_def);
            }
            visit_struct_definition(field_struct_def);
        }
        else {
            throw std::runtime_error("UNEXPECTED TYPE (maybe union ? ?): " + field.field_type.to_string());
        }
    }
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
    else if (!type_signature.is<PrimitiveType>()) {
        std::ignore = types_register.retrieve(type_signature);
    }
}
