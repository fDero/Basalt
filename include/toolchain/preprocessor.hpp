
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
        TypeDependencyNavigator(TypeDefinitionsRegister& types_register);
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
        void instantiation_and_visit_union(const UnionDefinition& union_definition, const TypeSignature& concrete_type);
        void instantiation_and_visit_struct(const StructDefinition& struct_definition, const TypeSignature& concrete_type);

        TypeDefinitionsRegister& types_register;
        std::unordered_set<std::string> visited_definitions;
};