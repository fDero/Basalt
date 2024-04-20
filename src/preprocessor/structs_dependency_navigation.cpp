
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
    else if (field.field_type.is_core_language_type()) {
        verify_that_the_type_exists(field.field_type);
    }
    else {
        TypeDefinition type_def = types_register.retrieve(field.field_type);
        if (type_def.is<StructDefinition>()){
            StructDefinition& struct_def = type_def.get<StructDefinition>();
            instanciate_and_visit_struct(struct_def, field.field_type);
        }
        else if (type_def.is<UnionDefinition>()) {
            UnionDefinition& alternative_def = type_def.get<UnionDefinition>();
            instanciate_and_visit_union(alternative_def, field.field_type);
        }
    }
}

void TypeDependencyNavigator::instanciate_and_visit_struct(
    const StructDefinition& struct_definition, 
    const TypeSignature& concrete_type
){
    assert_typesignature_is<CustomType>(concrete_type);
    if (struct_definition.template_generics_names.empty()){
        visit_struct_definition(struct_definition);
        return;
    }
    StructDefinition instanciated_struct = struct_definition;
    instanciated_struct.instanciate_generics(concrete_type.get<CustomType>());
    types_register.store(instanciated_struct);
    visit_struct_definition(instanciated_struct);
}