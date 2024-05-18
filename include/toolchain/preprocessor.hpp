
#pragma once
#include "toolchain/tokenizer.hpp"
#include "toolchain/parser.hpp"
#include "language/syntax.hpp"
#include <vector>
#include <string>
#include <variant>
#include <unordered_set>

class TypeDependencyNavigator {

    public:
        TypeDependencyNavigator(ProgramRepresentation& program_representation);
        void visit_struct_definition(const StructDefinition& struct_definition);
        void visit_union_definition(const UnionDefinition& union_definition);
        
    private:

        void visit_type_definition(
            const TypeSignature& typesignature,
            const TypeDefinition& type_definition,
            const std::vector<std::string>& generics
        );

        void visit_typesignature(const TypeSignature& typesignature, const std::vector<std::string>& union_def_generics);
        void verify_that_the_type_exists(const TypeSignature& type_signature);

        ProgramRepresentation& program_representation;
        std::unordered_set<std::string> visited_definitions;
};

class PackageTypeConflictNavigator {
    
        public:
            PackageTypeConflictNavigator(ProgramRepresentation& program_representation);
            void visit_file(const Filerepresentation& file_representation);
            void visit_package(const PackageName& package_name);
            
        private:
            ProgramRepresentation& program_representation;
            std::unordered_set<FileName> visited_files;
            std::unordered_set<std::string> type_definition_match_patterns;
};