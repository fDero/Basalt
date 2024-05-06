
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

void TypeDependencyNavigator::visit_struct_definition(const StructDefinition& struct_definition){
    std::string struct_id = struct_definition.generate_struct_id();
    ensure_type_not_already_visited_hence_no_cyclic_dependency(struct_id, visited_definitions);
    visited_definitions.insert(struct_id);
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
    else if (field.field_type.is<PointerType>() || field.field_type.is<SliceType>()) {
        verify_that_the_type_exists(field.field_type);
    }
    else if (field.field_type.is<ArrayType>()) {
        const TypeSignature& stored_type = field.field_type.get<ArrayType>().stored_type;
        visit_typesignature(stored_type);
    }
    else if (!field.field_type.is_primitive_type()) {
        visit_typesignature(field.field_type);
    }
}

void TypeDependencyNavigator::instantiation_and_visit_struct(
    const StructDefinition& struct_definition, 
    const TypeSignature& concrete_type
){
    assert_typesignature_is<BaseType>(concrete_type);
    if (struct_definition.template_generics_names.empty()){
        visit_struct_definition(struct_definition);
        return;
    }
    StructDefinition instantiationd_struct = struct_definition;
    instantiationd_struct.instantiate_generics(concrete_type.get<BaseType>());
    types_register.store(instantiationd_struct);
    visit_struct_definition(instantiationd_struct);
}