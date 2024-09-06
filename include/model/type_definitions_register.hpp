
#pragma once

#include <string>
#include <unordered_map>

#include "model/project_file_structure.hpp"
#include "language/definitions.hpp"
#include "language/typesignatures.hpp"

class TypeDefinitionsRegister {

    public:
        TypeDefinitionsRegister(ProjectFileStructure& project_file_structure);
        void store_type_definition(const TypeDefinition& type_definition);
        void verify_that_the_type_exists(const TypeSignature& type_signature);
        void foreach_type_definition(std::function<void(const TypeDefinition&)> visitor);
        [[nodiscard]] std::string get_fully_qualified_typesignature_name(const TypeSignature& type_signature);
        [[nodiscard]] TypeDefinition retrieve_type_definition(const CustomType& type_signature);
        [[nodiscard]] TypeSignature unalias_type(const TypeSignature& type_signature);
        
    protected:
        [[nodiscard]] std::string get_fully_qualified_customtype_name(const CustomType& type_signature);
        [[nodiscard]] std::optional<std::string> search_fully_qualified_typesignature_name(const CustomType&, const std::string&);
        [[nodiscard]] std::string get_type_definition_match_pattern(const std::string&, const TypeDefinition&);
        [[nodiscard]] std::string get_type_signature_match_pattern(const std::string&, const CustomType&);
        [[nodiscard]] std::string infer_possible_fully_qualified_typesignature_name(const std::string&, const TypeSignature&);
        [[nodiscard]] std::string infer_possible_fully_qualified_customtype_name(const std::string &packageName, const CustomType &custom_type);

    private:
        std::list<std::string> type_definitions_ids;
        std::unordered_map<std::string, TypeDefinition> type_definitions;
        ProjectFileStructure& project_file_structure;

    friend class TypeDependencyNavigator;
    friend class PackageTypeConflictNavigator;
    friend class AssignmentTypeChecker;
};