/**
 * @file overloading_resolution_engine.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the OverloadingResolutionEngine class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <unordered_map>

#include "model/project_file_structure.hpp"
#include "model/function_overloads_register.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "typesystem/generics_substitution_rules.hpp"


/**
 * @brief   Used to retrieve a function-definition-shared_ptr from a function call. In case of overloads, it will
 *          select the most appropriate one. In case of ambiguity, it will raise an error, in case of
 *          no match, it will return a nullptr.
 * 
 * @details The OverloadingResolutionEngine class is used to retrieve a function-definition-shared_ptr from a function call.
 *          In case of overloads, it will select the most appropriate one. In case of ambiguity, it will raise an error. 
 *          
 * @note    In case of no match, it will return a nullptr so that function-definition-retrieval can fail without errors,
 *          allowing the caller to attempt a common-feature-adoption resolution (It is a Basalt feature that basically allows
 *          Basalt to infer the definition of a non-existing overload from the context enabling its auto-generation).
 * 
 * @see     FunctionOverloadsRegister
 * @see     FunctionSpecificityDescriptor
 *
 */
class OverloadingResolutionEngine {

    public:

        /**
         * @brief Construct a new Overloading Resolution Engine object
         * 
         * @param function_overloads_register used to initialize the internal function_overloads_register attribute.
         * 
         * @param type_definitions_register used to initialize the internal type_definitions_register attribute.
         * 
         * @param project_file_structure used to initialize the internal project_file_structure attribute.
         */
        OverloadingResolutionEngine(
            FunctionOverloadsRegister& function_overloads_register, 
            TypeDefinitionsRegister& type_definitions_register,
            ProjectFileStructure& project_file_structure
        );

        /**
         * @brief   Used to retrieve a function-definition-shared_ptr from a function call. 
         *          In case of overloads, it will select the most appropriate one comparing
         *          their specificity using the FunctionSpecificityDescriptor class.
         * 
         * @note    This method relies on caching of previous retrievals to speed up the process.
         *          If the function call has already been resolved, it will return the cached result.
         *          If the function call has not been resolved yet, it will call the 
         *          cache_unaware_function_definition_retrieval
         * 
         * @param   function_call the function call to resolve.
         *  
         * @param   arg_types the types of the arguments of the function call.
         *  
         * @return  FunctionDefinition::Ref A shared_ptr to the function definition that 
         *          best matches the function call. If no match is found, it will return a nullptr.
         * 
         * @see     FunctionSpecificityDescriptor
         */
        FunctionDefinition::Ref retrieve_function_definition(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

        /**
         * @brief   Used to check if a function definition is compatible with a function call.
         * 
         * @param   func_def_ref the function definition to check compatibility with.
         * 
         * @param   func_call the function call to check compatibility with.
         * 
         * @param   arg_types the types of the arguments of the function call.
         * 
         * @return  GenericSubstitutionRule::Set::Ref A shared_ptr to the set of generic substitution rules
         *          that can be applied to the function definition to make it compatible with the function call.
         *          If the function definition is not compatible with the function call, it will return a nullptr.
         */
        GenericSubstitutionRule::Set::Ref check_function_compatibility(
            const FunctionDefinition::Ref func_def_ref,
            const FunctionCall& func_call,
            const std::vector<TypeSignature>& arg_types
        );

    private:

        /**
         * @brief   Used to retrieve a function-definition-shared_ptr from a function call. 
         *          In case of overloads, it will select the most appropriate one comparing
         *          their specificity using the FunctionSpecificityDescriptor class.
         * 
         * @param   function_call the function call to resolve.
         *  
         * @param   arg_types the types of the arguments of the function call.
         *  
         * @return  FunctionDefinition::Ref A shared_ptr to the function definition that 
         *          best matches the function call. If no match is found, it will return a nullptr.
         * 
         * @see     FunctionSpecificityDescriptor
         */
        FunctionDefinition::Ref cache_unaware_function_definition_retrieval(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

        /**
         * @brief   Used to retrieve whole overload-sets from wich attempt to retrieve 
         *          the most appropriate function definitions for the given function calls.
         */
        FunctionOverloadsRegister& function_overloads_register;
        
        /**
         * @brief    Used to initialize an instance of AssignmentTypeChecker to check the compatibility
         *           of the function definition with the function call testing the assignability of
         *           the arguments of the function call to the parameters of the function definition.
         *           Also used to initialize some functions specificities descriptors.         
         */
        TypeDefinitionsRegister& type_definitions_register;

        /**
         * @brief    Used to initialize an instance of AssignmentTypeChecker to check the compatibility
         *           of the function definition with the function call testing the assignability of
         *           the arguments of the function call to the parameters of the function definition.
         */
        ProjectFileStructure& project_file_structure;
        
        /**
         * @brief    Used to cache the results of previous overload resolutions to speed up the process.
         */
        std::unordered_map<std::string, FunctionDefinition::Ref> fast_retrieve_cache;
};