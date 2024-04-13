
#include "toolchain/rappresentation.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "errors/parsing_errors.hpp"
#include "language/syntax.hpp"
#include <iostream>

void StructDefinitionsRegister::store(const StructDefinition& struct_def){
    const std::string& struct_tag_name = struct_def.pattern_tag_name;
    std::cerr << "storing this tag name: " << struct_tag_name << "\n";
    ensure_struct_doesnt_already_exists(struct_tag_name, struct_def, struct_definitions);
    struct_definitions.insert(std::make_pair(struct_tag_name, struct_def));
}

[[nodiscard]] const StructDefinition& StructDefinitionsRegister::retrieve(const TypeSignature& type_signature) const {
    auto search_outcome = struct_definitions.find(type_signature.to_string());
    if (search_outcome != struct_definitions.end()) {
        return search_outcome->second;
    }
    search_outcome = struct_definitions.find(type_signature.struct_retrieval_match_string());
    if (search_outcome != struct_definitions.end()) {
        return search_outcome->second;
    }
    throw std::runtime_error("NO STRUCT FOUND: " + type_signature.to_string() + " " + type_signature.struct_retrieval_match_string());
}