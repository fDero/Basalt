//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <unordered_set>

#include "core/program_representation.hpp"

class PackageTypeConflictNavigator {
    
    public:
        PackageTypeConflictNavigator(ProgramRepresentation& program_representation);
        
        void visit_package(const std::string& package_name);
        void visit_all_packages();

    private:
        struct SinglePackageTypeConflictNavigator {
            SinglePackageTypeConflictNavigator(
                ProgramRepresentation& program_representation
            );

            void visit_package(const std::string& package_name);
            void visit_file(const FileRepresentation& file_representation);

            [[nodiscard]] std::string get_type_definition_conflict_detection_pattern(
                const TypeDefinition& type_definition
            );

            std::unordered_set<std::string> visited_files;
            std::unordered_set<std::string> type_definition_conflict_detection_patterns;
            ProgramRepresentation& program_representation;
        };

        ProgramRepresentation& program_representation;
};