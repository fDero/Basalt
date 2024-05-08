
#include "toolchain/rappresentation.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"

void TypeDefinitionsRegister::store(const TypeDefinition& type_def){
    if (type_def.is<StructDefinition>()) {
       store(type_def.get<StructDefinition>());
        return;
    } 
    if (type_def.is<UnionDefinition>()) {
       store(type_def.get<UnionDefinition>());
        return;
    }
    if (type_def.is<TypeAlias>()) {
        store(type_def.get<TypeAlias>());
        return;
    }
    assert_unreachable();
}

[[nodiscard]] const TypeDefinition& TypeDefinitionsRegister::direct_retrieve(const TypeSignature& type_signature) {
    auto search_outcome = type_definitions.find(type_signature.to_string());
    if (search_outcome != type_definitions.end()) {
        return search_outcome->second;
    }
    search_outcome = type_definitions.find(type_signature.to_match_string());
    if (search_outcome != type_definitions.end()) {
        TypeDefinition generic_def = search_outcome->second;
        generic_def.instantiate_generics(type_signature.get<BaseType>());
        std::string match_pattern = generic_def.generate_match_pattern();
        type_definitions.insert(std::make_pair(match_pattern, generic_def));
        return type_definitions.at(match_pattern);
    }
    throw std::runtime_error("NO TYPE FOUND: " + type_signature.to_string() + " " + type_signature.to_match_string());
}

[[nodiscard]] const TypeDefinition& TypeDefinitionsRegister::retrieve(const TypeSignature& type_signature) {
    const TypeDefinition& retrieved_type_definition = direct_retrieve(type_signature);
    return (retrieved_type_definition.is<TypeAlias>()) 
        ? retrieve(retrieved_type_definition.get<TypeAlias>().aliased_type) 
        : retrieved_type_definition;
}