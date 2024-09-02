/**
 * @file type_dependency_navigator.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the TypeDependencyNavigator class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "model/type_definitions_register.hpp"
#include "language/definitions.hpp"

/**
 * @brief   Used to verify that a given type definition is valid, meaning it and its dependencies exist, 
 *          and they don't lead to cyclic direct dependencies.  
 * 
 * @details Upon construction, the TypeDependencyNavigator is given a reference to an object of type
 *          TypeDefinitionsRegister, which contains all the type definitions of the program. Then 
 *          it exposes an API with two functionalities: checking if a type definition exists and
 *          checking that the type definition is correct. Being correct means that the type definition
 *          does not uses non-existing types, and that it uses them in such a way that they do not 
 *          create a cyclic direct dependencies-graph.
 */
class TypeDependencyNavigator {

    public:
        TypeDependencyNavigator(TypeDefinitionsRegister& program_representation);
        void visit_struct_definition(const StructDefinition& struct_definition);
        void visit_union_definition(const UnionDefinition& union_definition);
        void visit_type_definition(const TypeDefinition& type_definition);
        void verify_that_the_type_exists(const TypeSignature& type_signature);

    private:
        void visit_type_definition(
            const TypeSignature& typesignature,
            const TypeDefinition& type_definition,
            const std::vector<std::string>& generics
        );

        void visit_typesignature(const TypeSignature& typesignature, const std::vector<std::string>& union_def_generics);

        TypeDefinitionsRegister& type_definitions_register;
        std::unordered_set<std::string> visited_definitions;
};