
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