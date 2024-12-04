//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "preprocessing/package_type_conflict_navigator.hpp"
#include "errors/preprocessing_errors.hpp"

using PTCN = PackageTypeConflictNavigator;

PackageTypeConflictNavigator::PackageTypeConflictNavigator(
    ProgramRepresentation& program_representation
) 
    : program_representation(program_representation) 
{}

void PackageTypeConflictNavigator::visit_package(const std::string& package_name) {
    SinglePackageTypeConflictNavigator single_package_type_conflict_navigator(program_representation);
    single_package_type_conflict_navigator.visit_package(package_name);
}

void PackageTypeConflictNavigator::visit_all_packages() {
    program_representation.foreach_package([&](const std::string& package_name) {
        visit_package(package_name);
    });
}

PTCN::SinglePackageTypeConflictNavigator::SinglePackageTypeConflictNavigator(
    ProgramRepresentation& program_representation
) 
    : program_representation(program_representation) 
{}

void PTCN::SinglePackageTypeConflictNavigator::visit_package(const std::string& package_name) {
    std::list<FileRepresentation>& files = program_representation.get_files_by_package(package_name);
    for (const FileRepresentation& file : files) {
        visit_file(file);
    }
}

void PTCN::SinglePackageTypeConflictNavigator::visit_file(const FileRepresentation& file_representation) {
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

std::string PTCN::SinglePackageTypeConflictNavigator::get_type_definition_conflict_detection_pattern(
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