
#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "frontend/file_representation.hpp"

class ProjectFileStructure {

    public:

        ProjectFileStructure() = default;
        ProjectFileStructure(const std::vector<FileRepresentation>& file_representations);

        void store_file_representation(const FileRepresentation& file_representation);
        
        std::string& get_package_name_by_file_name(const std::string& file_name);
        std::vector<FileRepresentation>& get_files_by_package(const std::string& package_name);
        std::vector<std::string>& get_imports_by_file(const std::string& file_name);
        
        const std::string& get_package_name_by_file_name(const std::string& file_name) const;
        const std::vector<FileRepresentation>& get_files_by_package(const std::string& package_name) const;
        const std::vector<std::string>& get_imports_by_file(const std::string& file_name) const;
        const std::unordered_map<std::string, std::vector<FileRepresentation>>& get_all_files_grouped_by_package() const;

    private:

        std::unordered_map<std::string, std::string> package_name_by_file_name;
        std::unordered_map<std::string, std::vector<FileRepresentation>> files_by_package;
        std::unordered_map<std::string, std::vector<std::string>> imports_by_file;
};