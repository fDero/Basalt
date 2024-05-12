
#pragma once
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "misc/aliases.hpp"
#include <unordered_map>
#include <map>

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


struct ProgramRappresentation {

    void store_definitions_from_file(const FileRappresentation& file_rappresentation);
    
    void store_type_definition(const TypeDefinition& type_definition, const PackageName& package_name);
    
    [[nodiscard]] TypeDefinition retrieve_type_definition(const BaseType& type_signature);
    
    [[nodiscard]] std::optional<TypeDefinition> attempt_to_retrieve_type_definition_from_package(
        const BaseType& type_signature, const PackageName& package_name
    );

    [[nodiscard]] TypeDefinition retrieve_type_definition_from_package(
        const BaseType& type_signature, const PackageName& package_name
    );

    
    using PackageTypeDefinitions = std::map<std::string, TypeDefinition>;
    std::unordered_map<FileName, PackageName> package_name_by_file_name;
    std::unordered_map<PackageName, std::vector<FileRappresentation>> files_by_package;
    std::unordered_map<FileName, std::vector<PackageName>> imports_by_file;
    std::unordered_map<PackageName, PackageTypeDefinitions> types_by_package;
};
