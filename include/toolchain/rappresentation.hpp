
#pragma once
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "misc/aliases.hpp"
#include <unordered_map>
#include <map>

class TypeDefinitionsRegister {
    
    public:

        void store(const TypeDefinition& type_def);
        [[nodiscard]] const TypeDefinition& retrieve(const TypeSignature& type_signature);
        [[nodiscard]] bool contains(const TypeSignature& type_signature);
        [[nodiscard]] const std::map<std::string, TypeDefinition>& get_definitions() const;


    private:        

        void store_struct_defintion(const StructDefinition& struct_def);
        void store_union_definition(const UnionDefinition& union_def);
        void store_type_alias_definition(const TypeAlias& alias_def);

        std::map<std::string, TypeDefinition> type_definitions;
};



struct FileRappresentation {

    struct Metadata {
        FileName filename;
        PackageName packagename;
        std::vector<PackageName> imports;
    };

    Metadata file_metadata;
    std::vector<TypeDefinition> type_defs;
    std::vector<FunctionDefinition> func_defs;
};




class ProgramRappresentation {

    public:
    
        void store_definitions_from_file(const FileRappresentation& file_rappresentation);
        [[nodiscard]] const TypeDefinition& retrieve_type_definition(const BaseType& type_signature);

    protected:
        
        std::unordered_map<FileName, PackageName> package_name_by_file_name;
        std::unordered_map<PackageName, std::vector<FileRappresentation>> files_by_package;
        std::unordered_map<FileName, std::vector<PackageName>> imports_by_file;
        std::unordered_map<PackageName, TypeDefinitionsRegister> types_by_package;

    friend class TypeConflictNavigator;
    friend class TypeDependencyNavigator;
};
