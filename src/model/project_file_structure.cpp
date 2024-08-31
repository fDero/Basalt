
#include "errors/preprocessing_errors.hpp"
#include "errors/internal_errors.hpp"

ProjectFileStructure::ProjectFileStructure(const std::vector<FileRepresentation>& file_representations) {
    for (const FileRepresentation& file_representation : file_representations) {
        store_file_representation(file_representation);
    }
}

void ProjectFileStructure::store_file_representation(const FileRepresentation& file_representation) {
    const std::string& package_name = file_representation.file_metadata.packagename;
    files_by_package[package_name].push_back(file_representation);
    package_name_by_file_name[file_representation.file_metadata.filename] = package_name;
    const std::string& filename = file_representation.file_metadata.filename;
    const std::vector<std::string>& imports = file_representation.file_metadata.imports;
    imports_by_file[filename] = imports;
}

std::string& ProjectFileStructure::get_package_name_by_file_name(const std::string& file_name) {
    auto search_outcome = package_name_by_file_name.find(file_name);
    assert_packagename_is_found(search_outcome, package_name_by_file_name.end());
    return search_outcome->second;
}

std::vector<FileRepresentation>& ProjectFileStructure::get_files_by_package(const std::string& package_name) {
    auto search_outcome = files_by_package.find(package_name);
    assert_files_vector_is_found(search_outcome, files_by_package.end());
    return search_outcome->second;
}

std::vector<std::string>& ProjectFileStructure::get_imports_by_file(const std::string& file_name) {
    auto search_outcome = imports_by_file.find(file_name);
    assert_imports_vector_is_found(search_outcome, imports_by_file.end());
    return search_outcome->second;
}

const std::string& ProjectFileStructure::get_package_name_by_file_name(const std::string& file_name) const {
    ProjectFileStructure* casted_this = const_cast<ProjectFileStructure*>(this);
    return casted_this->get_package_name_by_file_name(file_name);
}

const std::vector<FileRepresentation>& ProjectFileStructure::get_files_by_package(const std::string& package_name) const {
    ProjectFileStructure* casted_this = const_cast<ProjectFileStructure*>(this);
    return casted_this->get_files_by_package(package_name);
}

const std::vector<std::string>& ProjectFileStructure::get_imports_by_file(const std::string& file_name) const {
    ProjectFileStructure* casted_this = const_cast<ProjectFileStructure*>(this);
    return casted_this->get_imports_by_file(file_name);
}

const std::unordered_map<std::string, std::vector<FileRepresentation>>& 
ProjectFileStructure::get_all_files_grouped_by_package() const {
    return files_by_package;
}