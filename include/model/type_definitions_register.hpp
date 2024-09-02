/**
 * @file type_definitions_register.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the TypeDefinitionsRegister class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <string>
#include <unordered_map>

#include "model/project_file_structure.hpp"
#include "language/definitions.hpp"
#include "language/typesignatures.hpp"

/**
 * @brief Used to keep track of all the type definitions of the program.
 * 
 * @details The TypeDefinitionsRegister is used to keep track of all the type definitions of the program.
 *          It exposes an API to store and retrieve type definitions, and to get the fully qualified name.
 *          It also provides a way to unalias a type, meaning to get the type definition of a type signature
 */
class TypeDefinitionsRegister {

    public:
    
        TypeDefinitionsRegister(ProjectFileStructure& project_file_structure);

        void store_type_definition(const TypeDefinition& type_definition);
        [[nodiscard]] TypeDefinition retrieve_type_definition(const CustomType& type_signature);
        [[nodiscard]] std::string get_fully_qualified_typesignature_name(const TypeSignature& type_signature);
        [[nodiscard]] TypeSignature unalias_type(const TypeSignature& type_signature);
        [[nodiscard]] std::unordered_map<std::string, TypeDefinition>& get_all_type_definitions();

    protected:
         
        [[nodiscard]] std::string get_fully_qualified_customtype_name(const CustomType& type_signature);
        [[nodiscard]] std::optional<std::string> search_fully_qualified_typesignature_name(const CustomType&, const std::string&);
        [[nodiscard]] std::string get_type_definition_match_pattern(const std::string&, const TypeDefinition&);
        [[nodiscard]] std::string get_type_signature_match_pattern(const std::string&, const CustomType&);
        [[nodiscard]] std::string infer_possible_fully_qualified_typesignature_name(const std::string&, const TypeSignature&);
        [[nodiscard]] std::string infer_possible_fully_qualified_customtype_name(const std::string &packageName, const CustomType &custom_type);

    private:

        std::unordered_map<std::string, TypeDefinition> type_definitions;
        ProjectFileStructure& project_file_structure;

    friend class TypeDependencyNavigator;
    friend class PackageTypeConflictNavigator;
    friend class AssignmentTypeChecker;
};