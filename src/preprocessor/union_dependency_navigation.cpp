
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

void TypeDependencyNavigator::visit_union_definition(const UnionDefinition& union_definition){
    std::string enum_id = union_definition.generate_union_id();
    ensure_type_not_already_visited_hence_no_cyclic_dependency(enum_id, visited_definitions);
    visited_definitions.insert(enum_id);
    for (const TypeSignature& alternative : union_definition.types){
        visit_typesignature( alternative, union_definition.template_generics_names );
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