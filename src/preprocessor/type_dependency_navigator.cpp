
#include "toolchain/preprocessor.hpp"
#include "errors/internal_errors.hpp"
#include "errors/parsing_errors.hpp"

void PreProcessor::preprocess_type_defintions(){
    for (const auto& struct_def_data : types_register.struct_definitions){
        TypeDependencyNavigator navigator(types_register);
        if (struct_def_data.second.is<StructDefinition>()){
            navigator.visit_struct_definition(struct_def_data.second.get<StructDefinition>());
        }
        else if (struct_def_data.second.is<UnionDefinition>()) {
            navigator.visit_union_definition(struct_def_data.second.get<UnionDefinition>());
        }
    }
}

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
    else if (type_signature.is<CustomType>()) {
        std::ignore = types_register.retrieve(type_signature);
    }
    else {
        assert_typesignature_is<PrimitiveType>(type_signature);
    }
}