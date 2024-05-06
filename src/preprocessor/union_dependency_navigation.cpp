
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
    else if (alternative.is<PointerType>() || alternative.is<SliceType>()) {
        verify_that_the_type_exists(alternative);
    }
    else if (alternative.is<ArrayType>()) {
        const TypeSignature& stored_type = alternative.get<ArrayType>().stored_type;
        visit_typesignature(stored_type);
    }
    else if (!alternative.is_primitive_type()) {
        visit_typesignature(alternative);
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