/**
 * @file function_overloads_register.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the FunctionOverloadsRegister class 
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "model/project_file_structure.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"

/**
 * @brief   Used to keep all of the function-overload-sets in the source code organized.
 * 
 * @details This class is used to keep all of the function-overload-sets in the source code organized. 
 *          It is used to store function definitions and retrieve the whole overload sets. It is used
 *          by the OverloadingResolutionEngine as the internal data structure to keep track of function
 *          definitions. 
 * 
 * @note    The retrieval of function definitions is done via the 'retrieve_specific_overload_set' method.
 *          It require the id of the overload set to be retrieved. Such an id can be inferred from the
 *          function call that is being resolved. It's not supposed to be used anywere in code but in the
 *          OverloadingResolutionEngine class, wich is the only class that have the capability to infer
 *          the correct overload set id from a function call.
 * 
 * @see     OverloadingResolutionEngine
 * 
 */
class FunctionOverloadsRegister {

    public:

        /**
         * @brief   Construct a new Function Overloads Register object
         * 
         * @param   project_file_structure used to initialize the internal project_file_structure attribute.
         * 
         * @note    During construction, the register will be populated with all the function definitions 
         *          found in the project_file_structure provided. But it will not be updated if new function
         *          definitions are added to the project_file_structure after the construction of the register.
         */
        FunctionOverloadsRegister(ProjectFileStructure& project_file_structure);
        
        /**
         * @brief   Used to store a function definition in the register. 
         * 
         * @details This method is used to store a function definition in the register. It will
         *          store the function definition in the correct overload set, based on the function
         *          name, the number of arguments and the number of template generics. Once a function definition
         *          is stored, it can not be retrieved directly. Instead the whole overload set can be retrieved
         *          using the 'retrieve_specific_overload_set' method. To retrieve such an overload set, its 
         *          overload-set id must be provided.
         *  
         * @note    If the function definition has template generics, it will be stored in both the overload set
         *          that takes into account the number of template generics and the overload set that does not.
         *          This is done to allow the OverloadingResolutionEngine to retrieve the correct definition
         *          regardless of type-inference being used or not in this function call.
         * 
         * @param   function_definition The function definition to store in the register.
         */
        void store_function_definition(
            const FunctionDefinition& function_definition
        );
        
        /**
         * @brief   Used to retrieve all the overload-set-ids that could be associated with a function call.
         * 
         * @details If the function definition can't be found in the overload-sets described by these ids,
         *          the function call is considered to be a call to a non-existing function.
         * 
         * @param   function_call the function call to get the overload-set-ids of.
         * 
         * @return  std::vector<std::string> all the overload-set-ids that could be associated with the function call.
         */
        [[nodiscard]] std::vector<std::string> retrieve_overload_sets_ids(
            const FunctionCall& function_call
        );

        /**
         * @brief  Used to retrieve a specific overload set from the register, given its id.  
         * 
         * @param  overload_set_id the id of the overload set to retrieve.
         * 
         * @return std::vector<FunctionDefinition::Ref>& the overload set that has been found.
         */
        [[nodiscard]] std::vector<FunctionDefinition::Ref>& retrieve_specific_overload_set(
            const std::string& overload_set_id
        );

        /**
         * @brief   Used to get the default overload-set-id of a function call. Meaning the overload-set-id
         *          relative to the package in wich the function call is located.
         * 
         * @param function_call the function call to get the default overload-set-id of.
         * 
         * @return std::string the default overload-set-id of the function call.
         */
        [[nodiscard]] std::string get_function_default_search_key(
            const FunctionCall& function_call
        );

        /**
         * @brief   Used to get all the function overload sets stored in the register.
         * 
         * @return  const std::unordered_map<std::string, FunctionDefinition::OverloadSet>& 
         *          all the function overload sets stored in the register.
         */
        [[nodiscard]] const std::unordered_map<std::string, FunctionDefinition::OverloadSet>& 
        get_all_function_overload_sets() const;

    protected:

        /**
         * @brief   Used to get the overload-set-id of a function definition, wich describes the first 
         *          overload-set where the function definition will stored.
         * 
         * @param   package_name The name of the package the function definition belongs to.
         * 
         * @param   function_definition The function definition to get the overload-set-id from. 
         * 
         * @return  std::string the overload-set-id describing the overload-set the function definition belongs to.
         */
        [[nodiscard]] std::string get_function_definition_overload_set_id(
            const std::string& package_name, 
            const FunctionDefinition::Ref function_definition
        );

        /**
         * @brief   Used to get the overload-set-id of a function definition, wich describes the 
         *          second overload-set where the function definition will stored.
         *  
         * @param   package_name The name of the package the function definition belongs to.
         * 
         * @param   function_definition The function definition to get the overload-set-id from. 
         * 
         * @return  std::string the overload-set-id describing the overload-set the function definition belongs to.
         */
        [[nodiscard]] std::string get_generics_unaware_function_definition_overload_set_id(
            const std::string& package_name, 
            const FunctionDefinition::Ref function_definition
        );

        /**
         * @brief   Used to get one of the possible overload-set-ids in wich the function definition
         *          related to the function call could be stored. Using different package names, it
         *          can be used to search such definition in different overload-sets.
         * 
         * @param   package_name 
         *              The package in wich the function definition is being searched. During overloading
         *              resolution, the first retrieve attempt is done in the package of the function call. 
         *              If no match is found, the search is expanded to the packages that are imported by the 
         *              file where the function call is located.
         * 
         * @param   original_function_call 
         *              The function call to get the overload-set-id of, assuming its definition is 
         *              indeed located in the package provided.
         * 
         * @return  std::string the overload-set-id that has been found.
         */
        [[nodiscard]] std::string get_function_call_overload_set_id(
            const std::string& package_name, 
            const FunctionCall& original_function_call
        );

    private:

        /**
         * @brief   used to extract the package a function call its located in from the filename
         *          of the file where the function call is located.
         */
        ProjectFileStructure& project_file_structure;

        /**
         * @brief   Used as the internal data structure to store all the function overload sets. 
         */
        std::unordered_map<std::string, FunctionDefinition::OverloadSet> function_definitions_overload_sets;
};