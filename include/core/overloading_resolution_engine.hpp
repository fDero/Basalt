//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <unordered_map>

#include "core/project_file_structure.hpp"
#include "core/function_overloads_register.hpp"
#include "core/type_definitions_register.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "core/generics_substitution_rules.hpp"
#include "core/caching_aware_register.hpp"

class OverloadingResolutionEngine : public CachingAwareRegister {

    public:
        OverloadingResolutionEngine(
            FunctionOverloadsRegister& function_overloads_register, 
            TypeDefinitionsRegister& type_definitions_register,
            ProjectFileStructure& project_file_structure
        );

        [[nodiscard]] FunctionDefinition::Ref retrieve_function_definition(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

        [[nodiscard]] GenericSubstitutionRule::Set::Ref check_function_compatibility(
            const FunctionDefinition::Ref func_def_ref,
            const FunctionCall& func_call,
            const std::vector<TypeSignature>& arg_types
        );

    protected:
        using CachingAwareRegister::get_cache_search_key_for_func_def_retrieval_from_func_call;
        using MatchedFunctionData = std::pair<FunctionDefinition::Ref, GenericSubstitutionRule::Set::Ref>;

        [[nodiscard]] std::vector<MatchedFunctionData> search_for_best_matches(
            const FunctionCall& function_call,
            const std::vector<TypeSignature>& arg_types
        );

        [[nodiscard]] std::string get_new_instantiated_function_name(
            const FunctionDefinition& function_definition,
            GenericSubstitutionRule::Set::Ref generic_substitution_rules
        );

        [[nodiscard]] FunctionDefinition::Ref cache_unaware_function_definition_retrieval(
            const FunctionCall& function_call, 
            const std::vector<TypeSignature>& arg_types
        );

    private:
        FunctionOverloadsRegister& function_overloads_register;
        TypeDefinitionsRegister& type_definitions_register;
        ProjectFileStructure& project_file_structure;
        std::unordered_map<std::string, FunctionDefinition::Ref> fast_retrieve_cache;
};