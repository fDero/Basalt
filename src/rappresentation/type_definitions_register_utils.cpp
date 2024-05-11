
#include "toolchain/rappresentation.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include "errors/internal_errors.hpp"

void TypeDefinitionsRegister::store(const TypeDefinition& type_def){
    if (type_def.is<StructDefinition>()) {
        store_struct_defintion(type_def.get<StructDefinition>());
        return;
    } 
    if (type_def.is<UnionDefinition>()) {
        store_union_definition(type_def.get<UnionDefinition>());
        return;
    }
    if (type_def.is<TypeAlias>()) {
        store_type_alias_definition(type_def.get<TypeAlias>());
        return;
    }
    assert_unreachable();
}

[[nodiscard]] const TypeDefinition& TypeDefinitionsRegister::retrieve(const TypeSignature& type_signature) {
    assert_typesignature_is<BaseType>(type_signature);
    std::string base_type_name = type_signature.get<BaseType>().type_name;
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

[[nodiscard]] bool TypeDefinitionsRegister::contains(const TypeSignature& type_signature) {
    try {
        std::ignore = retrieve(type_signature);
        return true;
    } catch (...) {
        return false;
    }
}

[[nodiscard]] const std::map<std::string, TypeDefinition>& TypeDefinitionsRegister::get_definitions() const {
    return type_definitions;
}