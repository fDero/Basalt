//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <list>

#include "frontend/file_representation.hpp"

class ProjectFileStructure {

    public:
        ProjectFileStructure() = default;
        ProjectFileStructure(const std::vector<FileRepresentation>& file_representations);

        void store_file_representation(const FileRepresentation& file_representation);
        void foreach_file(const std::function<void(const FileRepresentation&)>& func);
        void foreach_package(const std::function<void(const std::string&)>& func);

        [[nodiscard]] std::string& get_package_name_by_file_name(const std::string& file_name);
        [[nodiscard]] std::list<FileRepresentation>& get_files_by_package(const std::string& package_name);
        [[nodiscard]] std::vector<std::string>& get_imports_by_file(const std::string& file_name);
        
    private:
        std::list<std::string> package_names;
        std::list<std::list<FileRepresentation>::iterator> files;
        std::unordered_map<std::string, std::string> package_name_by_file_name;
        std::unordered_map<std::string, std::list<FileRepresentation>> files_by_package;
        std::unordered_map<std::string, std::vector<std::string>> imports_by_file;
};