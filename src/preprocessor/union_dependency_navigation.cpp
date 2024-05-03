
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

void TypeDependencyNavigator::visit_union_definition(const UnionDefinition& union_definition){
    std::string enum_id = union_definition.generate_union_id();
    ensure_type_not_already_visited_hence_no_cyclic_dependency(enum_id, visited_definitions);
    visited_definitions.insert(enum_id);
    for (const TypeSignature& alternative : union_definition.types){
        visit_union_alternative( alternative, union_definition.template_generics_names );
    }
}

void TypeDependencyNavigator::visit_union_alternative(
    const TypeSignature& alternative, 
    const std::vector<std::string>& struct_def_generics
){
    if (alternative.is_generic(struct_def_generics)) {
        return;
    }
    else if (alternative.is_core_language_type()) {
        verify_that_the_type_exists(alternative);
    }
    else {
        TypeDefinition type_def = types_register.retrieve(alternative);
        if (type_def.is<StructDefinition>()){
            StructDefinition& struct_def = type_def.get<StructDefinition>();
            instantiation_and_visit_struct(struct_def, alternative);
        }
        else if (type_def.is<UnionDefinition>()) {
            UnionDefinition& alternative_def = type_def.get<UnionDefinition>();
            instantiation_and_visit_union(alternative_def, alternative);
        }
    }
}

void TypeDependencyNavigator::instantiation_and_visit_union(
    const UnionDefinition& union_definition, 
    const TypeSignature& concrete_type
){
    assert_typesignature_is<BaseType>(concrete_type);
    if (union_definition.template_generics_names.empty()){
        visit_union_definition(union_definition);
        return;
    }
    UnionDefinition instantiationd_union = union_definition;   
    instantiationd_union.instantiate_generics(concrete_type.get<BaseType>());
    types_register.store(instantiationd_union);
    visit_union_definition(instantiationd_union);
}