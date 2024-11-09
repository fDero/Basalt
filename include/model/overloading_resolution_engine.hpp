//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <unordered_map>

#include "model/project_file_structure.hpp"
#include "model/function_overloads_register.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "typesystem/generics_substitution_rules.hpp"

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

        GenericSubstitutionRule::Set::Ref check_function_compatibility(
            const FunctionDefinition::Ref func_def_ref,
            const FunctionCall& func_call,
            const std::vector<TypeSignature>& arg_types
        );

    private:
        FunctionDefinition::Ref cache_unaware_function_definition_retrieval(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

        std::string get_function_default_search_key(
            const FunctionCall &function_call, 
            const std::vector<TypeSignature> &arg_types
        );

        FunctionOverloadsRegister& function_overloads_register;
        TypeDefinitionsRegister& type_definitions_register;
        ProjectFileStructure& project_file_structure;
        std::unordered_map<std::string, FunctionDefinition::Ref> fast_retrieve_cache;
};