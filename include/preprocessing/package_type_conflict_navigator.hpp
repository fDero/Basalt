//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <unordered_set>

#include "core/project_file_structure.hpp"

class PackageTypeConflictNavigator {
    
    public:
        PackageTypeConflictNavigator(ProjectFileStructure& project_file_structure);
        void visit_file(const FileRepresentation& file_representation);
        void visit_package(const std::string& package_name);
        
    private:
        ProjectFileStructure& project_file_structure;
        std::unordered_set<std::string> visited_files;
        std::unordered_set<std::string> type_definition_conflict_detection_patterns;

        [[nodiscard]] std::string get_type_definition_conflict_detection_pattern(
            const TypeDefinition& type_definition
        );
};