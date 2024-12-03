//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"
#include "core/project_file_structure.hpp"

#include <functional>

ProjectFileStructure::ProjectFileStructure(const std::vector<FileRepresentation>& file_representations) {
    for (const FileRepresentation& file_representation : file_representations) {
        store_file_representation(file_representation);
    }
}

void ProjectFileStructure::foreach_file(const std::function<void(const FileRepresentation&)>& func) {
    for (const std::list<FileRepresentation>::iterator& file_iterator : files) {
        const FileRepresentation& file_representation = *file_iterator;
        func(file_representation);
    }
}

void ProjectFileStructure::foreach_package(const std::function<void(const std::string&)>& func) {
    for (const std::string& package_name : package_names) {
        func(package_name);
    }
}

void ProjectFileStructure::store_file_representation(const FileRepresentation& file_representation) {
    const std::string& package_name = file_representation.file_metadata.packagename;
    files_by_package[package_name].push_front(file_representation);
    files.push_front(files_by_package[package_name].begin());
    package_name_by_file_name[file_representation.file_metadata.filename] = package_name;
    const std::string& filename = file_representation.file_metadata.filename;
    const std::vector<std::string>& imports = file_representation.file_metadata.imports;
    imports_by_file[filename] = imports;
    auto package_name_search_outcome = std::find(package_names.begin(), package_names.end(), package_name);
    if (package_name_search_outcome == package_names.end()) {
        package_names.push_front(package_name);
    }
}

std::string& ProjectFileStructure::get_package_name_by_file_name(const std::string& file_name) {
    auto search_outcome = package_name_by_file_name.find(file_name);
    assert_packagename_is_found(search_outcome, package_name_by_file_name.end());
    return search_outcome->second;
}

std::list<FileRepresentation>& ProjectFileStructure::get_files_by_package(const std::string& package_name) {
    auto search_outcome = files_by_package.find(package_name);
    assert_files_are_found(search_outcome, files_by_package.end());
    return search_outcome->second;
}

std::vector<std::string>& ProjectFileStructure::get_imports_by_file(const std::string& file_name) {
    auto search_outcome = imports_by_file.find(file_name);
    assert_imports_vector_is_found(search_outcome, imports_by_file.end());
    return search_outcome->second;
}