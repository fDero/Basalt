
#pragma once
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include <unordered_map>
#include <map>

struct TypeDefinitionsRegister {
    
    void store(const TypeDefinition& type_def);
    void store(const StructDefinition& struct_def);
    void store(const UnionDefinition& union_def);

    [[nodiscard]] const TypeDefinition& retrieve(const TypeSignature& type_signature) const;

    std::map<std::string, TypeDefinition> struct_definitions;
};

struct FileMetaData {
    std::string filename;
};

struct FileRappresentation {
    FileMetaData file_metadata;
    std::vector<TypeDefinition> type_defs;
    std::vector<FunctionDefinition> func_defs;
};