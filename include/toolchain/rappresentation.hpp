
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
    
        void store_definitions_from_file(const FileRappresentation& file_rappresentation){
            const std::string& package_name = file_rappresentation.file_metadata.packagename;
            files_by_package[package_name].push_back(file_rappresentation);
            package_name_by_file[file_rappresentation.file_metadata.filename] = package_name;
            for (const TypeDefinition& type_def : file_rappresentation.type_defs){
                types_by_package[package_name].store(type_def);
            }
            imports_by_file[file_rappresentation.file_metadata.filename] = 
                file_rappresentation.file_metadata.imports;
        }

        [[nodiscard]] const TypeDefinition& retrieve_type_definition(const BaseType& type_signature){
            if (!type_signature.package_prefix.empty()){
                return types_by_package.at(type_signature.package_prefix).retrieve(type_signature);
            }
            const PackageName& target_package_name = package_name_by_file.at(type_signature.filename);
            TypeDefinitionsRegister* reg = &(types_by_package[target_package_name]);
            auto import_iterator = imports_by_file[type_signature.filename].begin();
            auto imports_end = imports_by_file[type_signature.filename].end();
            while (!reg->contains(type_signature) && import_iterator != imports_end){
                reg = &(types_by_package[*import_iterator]);
                import_iterator++;
            }
            return reg->retrieve(type_signature);
        }

    private:
        
        std::unordered_map<FileName, PackageName> package_name_by_file;
        std::unordered_map<PackageName, std::vector<FileRappresentation>> files_by_package;
        std::unordered_map<FileName, std::vector<PackageName>> imports_by_file;
        std::unordered_map<FileName, TypeDefinitionsRegister> types_by_package;
};
