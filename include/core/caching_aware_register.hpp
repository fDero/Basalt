//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/type_definitions_register.hpp"
#include "language/functions.hpp"

class CachingAwareRegister {
    protected:
        [[nodiscard]] std::string get_cache_search_key_for_func_def_retrieval_from_func_call(
            TypeDefinitionsRegister& type_definitions_register,
            const FunctionCall& function_call,
            const std::vector<TypeSignature>& arg_types
        );

        [[nodiscard]] std::string get_function_definition_overload_set_id(
            const std::string& package_name, 
            const FunctionDefinition::Ref function_definition_ref
        );

        [[nodiscard]] std::string get_generics_unaware_function_definition_overload_set_id(
            const std::string& package_name, 
            const FunctionDefinition::Ref function_definition_ref
        );

        [[nodiscard]] std::string get_function_call_overload_set_id(
            const std::string& package_name, 
            const FunctionCall& original_function_call
        );

        [[nodiscard]] std::string get_overload_set_default_search_key(
            ProjectFileStructure project_file_structure, 
            const FunctionCall& function_call
        );
};