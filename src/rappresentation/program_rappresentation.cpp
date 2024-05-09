
#include "toolchain/rappresentation.hpp"

const TypeDefinition& ProgramRappresentation::retrieve_type_definition(const BaseType& type_signature){
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

void ProgramRappresentation::store_definitions_from_file(const FileRappresentation& file_rappresentation){
    const std::string& package_name = file_rappresentation.file_metadata.packagename;
    files_by_package[package_name].push_back(file_rappresentation);
    package_name_by_file[file_rappresentation.file_metadata.filename] = package_name;
    for (const TypeDefinition& type_def : file_rappresentation.type_defs){
        types_by_package[package_name].store(type_def);
    }
    imports_by_file[file_rappresentation.file_metadata.filename] = 
        file_rappresentation.file_metadata.imports;
}