/**
 * @file overloading_resolution_engine.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the OverloadingResolutionEngine class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <unordered_map>

#include "model/project_file_structure.hpp"
#include "model/function_overloads_register.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "language/generics.hpp"

/**
 * @brief   Used to retrieve a function-definition-shared_ptr from a function call. In case of overloads, it will
 *          select the most appropriate one. In case of ambiguity, it will raise an error, in case of
 *          no match, it will return a nullptr.
 * 
 * @details The OverloadingResolutionEngine class is used to retrieve a function-definition-shared_ptr from a function call.
 *          In case of overloads, it will select the most appropriate one. In case of ambiguity, it will raise an error. 
 *          
 * 
 * @note    In case of no match, it will return a nullptr so that function-definition-retrieval can fail without errors,
 *          allowing the caller to attempt a common-feature-adoption resolution (It is a Basalt feature that basically allows
 *          Basalt to infer the definition of a non-existing overload from the context enabling its auto-generation).
 * 
 * @see     FunctionOverloadsRegister
 * @see     FunctionSpecificityDescriptor
 */
class OverloadingResolutionEngine {

    public:
        OverloadingResolutionEngine(
            FunctionOverloadsRegister& function_overloads_register, 
            TypeDefinitionsRegister& type_definitions_register,
            ProjectFileStructure& project_file_structure
        );

        FunctionDefinition::Ref retrieve_function_definition(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

        GenericSubstitutionRuleSet::Ref check_function_compatibility(
            const FunctionDefinition::Ref func_def_ref,
            const FunctionCall& func_call,
            const std::vector<TypeSignature>& arg_types
        );

    protected:
        FunctionDefinition::Ref cache_unaware_function_definition_retrieval(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

    private:
        FunctionOverloadsRegister& function_overloads_register;
        TypeDefinitionsRegister& type_definitions_register;
        ProjectFileStructure& project_file_structure;
        std::unordered_map<std::string, FunctionDefinition::Ref> fast_retrieve_cache;
};