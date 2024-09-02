/**
 * @file package_type_conflict_navigator.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the PackageTypeConflictNavigator class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <string>
#include <unordered_set>

#include "model/project_file_structure.hpp"

/**
 * @brief   Used to detect if from a given file, two packages are imported with two 
 *          different type-definitions with a conflicting name.
 * 
 * @details The PackageTypeConflictNavigator is used to detect  if from a given file, 
 *          two packages are imported with two different type-definitions with a conflicting name.
 *           
 * 
 * @note    In case two type-definitions with the same name are found within the same package,
 *          that means that the program is in an invalid state, because the TypeDefinitionRegister
 *          itself should have not allowed such a situation to happen. This class will still be able
 *          to detect the error and will still behave as expected. 
 */
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