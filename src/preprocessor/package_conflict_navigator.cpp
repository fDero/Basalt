//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "model/project_file_structure.hpp"
#include "preprocessing/package_type_conflict_navigator.hpp"
#include "errors/preprocessing_errors.hpp"

PackageTypeConflictNavigator::PackageTypeConflictNavigator(ProjectFileStructure& project_file_structure) 
    : project_file_structure(project_file_structure) {}

void PackageTypeConflictNavigator::visit_file(const FileRepresentation& file_representation) {
    if (visited_files.find(file_representation.file_metadata.filename) != visited_files.end()) {
        return;
    }
    for (const TypeDefinition& type_def : file_representation.type_defs) {
        std::string conflict_detection_pattern = get_type_definition_conflict_detection_pattern(type_def);
        auto insertion_outcome = type_definition_conflict_detection_patterns.insert(conflict_detection_pattern);
        ensure_no_type_definition_conflict_detected(insertion_outcome);
    }
    visited_files.insert(file_representation.file_metadata.filename);
    for (const std::string& import : file_representation.file_metadata.imports) {
        visit_package(import);
    }
}

void PackageTypeConflictNavigator::visit_package(const std::string& package_name) {
    for (const FileRepresentation& file : project_file_structure.get_files_by_package(package_name)) {
        visit_file(file);
    }
}

std::string PackageTypeConflictNavigator::get_type_definition_conflict_detection_pattern(
    const TypeDefinition& type_definition
) {
    std::string pattern_tag_name = type_definition.get_simple_name();
    size_t number_of_generics = type_definition.get_number_of_generics();
    if (number_of_generics > 0) {
        pattern_tag_name += "<";
        for (size_t i = 0; i < number_of_generics; i++) {
            pattern_tag_name += "?,";
        }
        pattern_tag_name.back() = '>';
    }
    return pattern_tag_name;
}
