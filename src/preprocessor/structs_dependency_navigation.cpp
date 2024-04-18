
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

void PreProcessor::preprocess_structs(){
    for (const auto& struct_def_data : structs_register.struct_definitions){
        StructDependencyNavigator struct_nav(structs_register);
        struct_nav.visit_struct_definition(struct_def_data.second);
    }
}

StructDependencyNavigator::StructDependencyNavigator(
    StructDefinitionsRegister& structs_register
) : structs_register(structs_register) {
    visited_structs.reserve(structs_register.struct_definitions.size());
}

void StructDependencyNavigator::visit_struct_definition(const StructDefinition& struct_definition){
    std::string struct_id = struct_definition.generate_struct_id();
    ensure_struct_not_already_visited_hence_no_cyclic_dependency(struct_id, visited_structs);
    visited_structs.insert(struct_id);
    for (const StructDefinition::Field& field : struct_definition.fields){
        visit_struct_field( field, struct_definition.template_generics_names );
    }
}

void StructDependencyNavigator::visit_struct_field(
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
        StructDefinition field_def = structs_register.retrieve(field.field_type);
        if (!field_def.template_generics_names.empty()){
            assert_typesignature_is<CustomType>(field.field_type);
            field_def.instanciate_generics(field.field_type.get<CustomType>());
            structs_register.store(field_def);
        }
        visit_struct_definition(field_def);
    }
}

void StructDependencyNavigator::verify_that_the_type_exists(const TypeSignature& type_signature){
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
        std::ignore = structs_register.retrieve(type_signature);
    }
}
