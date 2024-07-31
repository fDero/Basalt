
#pragma once
#include "toolchain/tokenizer.hpp"
#include "toolchain/parser.hpp"
#include "language/syntax.hpp"
#include "language/generics.hpp"
#include <vector>
#include <string>
#include <variant>
#include <unordered_set>

class TypeDependencyNavigator {

    public:
        TypeDependencyNavigator(TypeDefinitionsRegister& program_representation);
        void visit_struct_definition(const StructDefinition& struct_definition);
        void visit_union_definition(const UnionDefinition& union_definition);
        void visit_type_definition(const TypeDefinition& type_definition);

    private:

        void visit_type_definition(
            const TypeSignature& typesignature,
            const TypeDefinition& type_definition,
            const std::vector<std::string>& generics
        );

        void visit_typesignature(const TypeSignature& typesignature, const std::vector<std::string>& union_def_generics);
        void verify_that_the_type_exists(const TypeSignature& type_signature);

        TypeDefinitionsRegister& type_definitions_register;
        std::unordered_set<std::string> visited_definitions;
};

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

class PreProcessor {

    public:
        PreProcessor(
            ProjectFileStructure& project_file_structure,
            TypeDefinitionsRegister& type_definitions_register,
            FunctionOverloadsRegister& function_overloads_register,
            OverloadingResolutionEngine& overloading_resolution_engine
        );

        void preprocess_packages_typename_conflicts();
        void preprocess_type_definitions();
    
    private:
        ProjectFileStructure& project_file_structure;
        TypeDefinitionsRegister& type_definitions_register;
        FunctionOverloadsRegister& function_overloads_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
};