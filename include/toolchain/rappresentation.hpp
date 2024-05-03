
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


class FunctionDefinitionsRegister {
    public:
        void store(const FunctionDefinition& func_def);
        
        [[nodiscard]] const FunctionDefinition& retrieve(
            const std::string& func_name, const std::vector<TypeSignature>& argtypes) const;

        std::unordered_map<std::string, FunctionDefinition> function_definitions_register;
        std::unordered_map<std::string, std::vector<std::string>> function_names_register;

    private:
        [[nodiscard]] static std::string craft_function_retrieve_string(
            const std::string& func_name, 
            const std::vector<TypeSignature>& argtypes);
};


struct FileMetaData {
    std::string filename;
};


struct FileRappresentation {
    FileMetaData file_metadata;
    std::vector<TypeDefinition> type_defs;
    std::vector<FunctionDefinition> func_defs;
};


struct ProgramRappresentation {
    std::vector<FileMetaData> file_metadata;
    TypeDefinitionsRegister type_defs_register;
    FunctionDefinitionsRegister func_defs_register;
};


struct ScopedData {
    std::vector<VariableDeclaration> variables;
    std::vector<ConstDeclaration> constants;

    using MaybeTypeSignature = std::optional<const TypeSignature&>;
    using MaybeVariableDeclaration = std::optional<const VariableDeclaration&>;
    using MaybeConstDeclaration = std::optional<const ConstDeclaration&>;

    [[nodiscard]] const TypeSignature& get_object_type_by_name(const std::string& name) const;
};