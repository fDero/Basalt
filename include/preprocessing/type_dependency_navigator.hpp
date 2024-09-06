
#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "model/type_definitions_register.hpp"
#include "language/definitions.hpp"

class TypeDependencyNavigator {

    public:
        TypeDependencyNavigator(TypeDefinitionsRegister& type_definition_register);
        void visit_struct_definition(const StructDefinition& struct_definition);
        void visit_union_definition(const UnionDefinition& union_definition);
        void visit_type_definition(const TypeDefinition& type_definition);
        void visit_all_type_definitions();

    private:
        void visit_type_definition(
            const TypeSignature& typesignature,
            const TypeDefinition& type_definition,
            const std::vector<std::string>& generics
        );

        void visit_typesignature(
            const TypeSignature& typesignature, 
            const std::vector<std::string>& union_def_generics
        );

        TypeDefinitionsRegister& type_definitions_register;
        std::unordered_set<std::string> visited_definitions;
};