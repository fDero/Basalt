
#pragma once
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include <unordered_map>
#include <map>

struct StructDefinitionsRegister {
    
    void store(const StructDefinition& struct_def);

    [[nodiscard]] const StructDefinition& retrieve(const TypeSignature& type_signature) const;

    std::map<std::string, StructDefinition> struct_definitions;
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
    StructDefinitionsRegister struct_defs_register;
    FunctionDefinitionsRegister func_defs_register;
};