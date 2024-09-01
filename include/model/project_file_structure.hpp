/**
 * @file project_file_structure.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the ProjectFileStructure class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "frontend/file_representation.hpp"

/**
 * @brief   Used to keep-track of the file structure of the codebase that is being compiled.
 *          It is used to enable easy access to the files, their packages and their dependencies.
 * 
 * @details The ProjectFileStructure is used to keep-track of the file structure of the codebase that is being compiled.
 *          It is used to enable easy access to the files, their packages and their dependencies, and it's used by
 *          most of the classes in the preprocessing and model modules.
 */
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