//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "core/program_representation.hpp"
#include "language/definitions.hpp"

class TypeDependencyNavigator {

    public:
        TypeDependencyNavigator(ProgramRepresentation& type_definition_register);
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

        ProgramRepresentation& program_representation;
        std::unordered_set<std::string> visited_definitions;
};