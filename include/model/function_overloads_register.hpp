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

        FunctionOverloadsRegister(ProjectFileStructure& project_file_structure);
        
        void store_function_definition(const FunctionDefinition& function_definition);
        
        [[nodiscard]] std::vector<std::string> retrieve_overload_sets_ids(const FunctionCall& function_call);
        [[nodiscard]] std::vector<FunctionDefinition::Ref>& retrieve_specific_overload_set(const std::string& overload_set_id);
        [[nodiscard]] std::string get_function_default_search_key(const FunctionCall& function_call);
        [[nodiscard]] const std::unordered_map<std::string, FunctionDefinition::OverloadSet>& get_all_function_overload_sets() const;

    protected:

        [[nodiscard]] std::string get_function_definition_overload_set_id(
            const std::string& package_name, 
            const FunctionDefinition::Ref function_definition
        );

        [[nodiscard]] std::string get_generics_unaware_function_definition_overload_set_id(
            const std::string& package_name, 
            const FunctionDefinition::Ref function_definition
        );

        [[nodiscard]] std::string get_function_call_overload_set_id(
            const std::string& package_name, 
            const FunctionCall& original_function_call
        );

    private:

        ProjectFileStructure& project_file_structure;
        std::unordered_map<std::string, FunctionDefinition::OverloadSet> function_definitions_overload_sets;
};