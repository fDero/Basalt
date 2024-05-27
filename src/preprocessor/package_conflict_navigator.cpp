
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
        std::string conflict_detection_pattern = get_type_definition_conflict_detection_pattern(type_def);
        auto insertion_outcome = type_definition_conflict_detection_patterns.insert(conflict_detection_pattern);
        ensure_no_type_definition_conflict_detected(insertion_outcome);
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

std::string PackageTypeConflictNavigator::get_type_definition_conflict_detection_pattern(
    const TypeDefinition& type_definition
){
    std::string pattern_tag_name = type_definition.get_simple_name();
    size_t number_of_generics = type_definition.get_number_of_generics();
    if (number_of_generics > 0) {
        pattern_tag_name += "<";
        for (int i = 0; i < number_of_generics; i++) { 
            pattern_tag_name += "?,";
        }
        pattern_tag_name.back() = '>';
    }
    return pattern_tag_name;
}