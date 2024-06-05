
#include "toolchain/representation.hpp"
#include "errors/preprocessing_errors.hpp"

void ProgramRepresentation::store_definitions_from_file(const Filerepresentation& file_representation) {
    const std::string& package_name = file_representation.file_metadata.packagename;
    files_by_package[package_name].push_back(file_representation);
    package_name_by_file_name[file_representation.file_metadata.filename] = package_name;
    for (const TypeDefinition& type_def : file_representation.type_defs) {
        store_type_definition(type_def, package_name);
    }
    for (const FunctionDefinition& func_def : file_representation.func_defs) {
        store_function_definition(func_def, package_name);
    }
    const std::string& filename = file_representation.file_metadata.filename;
    const std::vector<std::string>& imports = file_representation.file_metadata.imports;
    imports_by_file[filename] = imports;
}