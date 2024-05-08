
#include "toolchain/rappresentation.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"

void TypeDefinitionsRegister::store(const TypeDefinition& type_def){
    if (type_def.is<StructDefinition>()) {
       store(type_def.get<StructDefinition>());
    } 
    else if (type_def.is<UnionDefinition>()) {
       store(type_def.get<UnionDefinition>());
    }
    else if (type_def.is<TypeAlias>()) {
        store(type_def.get<TypeAlias>());
    }
    else {
        throw std::runtime_error("Unknown type definition type");
    }
}

[[nodiscard]] const TypeDefinition& TypeDefinitionsRegister::retrieve(const TypeSignature& type_signature) const {
    auto search_outcome = struct_definitions.find(type_signature.to_string());
    if (search_outcome != struct_definitions.end()) {
        return search_outcome->second;
    }
    search_outcome = struct_definitions.find(type_signature.to_match_string());
    if (search_outcome != struct_definitions.end()) {
        return search_outcome->second;
    }
    throw std::runtime_error("NO TYPE FOUND: " + type_signature.to_string() + " " + type_signature.to_match_string());
}