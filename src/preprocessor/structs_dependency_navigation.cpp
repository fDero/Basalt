
#include "toolchain/preprocessor.hpp"
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
    ensure_struct_not_already_visited_hence_no_cyclic_dependency(struct_definition.struct_id, visited_structs);
    visited_structs.insert(struct_definition.struct_id);
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
    else if (field.field_type.is<PointerType>()) {
        verify_that_the_type_exists(
            field.field_type.get<PointerType>().pointed_type);
    }
    else if (field.field_type.is<ArrayType>()) {
        verify_that_the_type_exists(
            field.field_type.get<ArrayType>().stored_type);
    }
    else if (field.field_type.is<SliceType>()) {
        verify_that_the_type_exists(
            field.field_type.get<SliceType>().stored_type);
    }
    else if (!field.field_type.is_primitive_type()) {
        const StructDefinition& field_def = structs_register.retrieve(field.field_type);
        visit_struct_definition(field_def);
    }
}

void StructDependencyNavigator::verify_that_the_type_exists(const TypeSignature& type_signature){
    const StructDefinition& field_def = structs_register.retrieve(type_signature);
}
