
#include "toolchain/representation.hpp"
#include "toolchain/preprocessor.hpp"
#include "errors/preprocessing_errors.hpp"

PackageTypeConflictNavigator::PackageTypeConflictNavigator(ProgramRepresentation& program_representation) 
    : program_representation(program_representation) {}

void PackageTypeConflictNavigator::visit_file(const Filerepresentation& file_representation) {
    if (visited_files.find(file_representation.file_metadata.filename) != visited_files.end()){
        return;
    }
    for (const TypeDefinition& type_def : file_representation.type_defs){
        std::string match_pattern = type_def.generate_match_pattern();
        if (type_definition_match_patterns.find(match_pattern) != type_definition_match_patterns.end()){
            throw std::runtime_error("Type conflict detected");
        }
        type_definition_match_patterns.insert(match_pattern);
    }
    visited_files.insert(file_representation.file_metadata.filename);
    for (const PackageName& import : file_representation.file_metadata.imports){
        visit_package(import);
    }
}

void PackageTypeConflictNavigator::visit_package(const PackageName& package_name) {
    for (const Filerepresentation& file : program_representation.files_by_package[package_name]){
        visit_file(file);
    }
}