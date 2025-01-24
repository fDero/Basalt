//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <unordered_map>

#include "core/project_file_structure.hpp"
#include "core/function_definitions_register.hpp"
#include "core/type_definitions_register.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "core/generics_substitution_rules.hpp"
#include "core/caching_aware_register.hpp"

class FunctionDefinitionsRegister : public CachingAwareRegister {

    public:
        FunctionDefinitionsRegister(
            TypeDefinitionsRegister& type_definitions_register,
            ProjectFileStructure& project_file_structure
        );

        void store_function_definition(
            const FunctionDefinition& function_definition
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

        void foreach_function_definition(
            std::function<void(FunctionDefinition::Ref)> functor
        );

        void foreach_main_function_definition(
            std::function<void(FunctionDefinition::Ref, std::string)> functor
        );

        [[nodiscard]] std::vector<std::string> retrieve_overload_sets_ids(
            const FunctionCall& function_call
        );

        [[nodiscard]] std::vector<FunctionDefinition::Ref>& retrieve_specific_overload_set(
            const std::string& overload_set_id
        );

    protected:
        using CachingAwareRegister::get_cache_search_key_for_func_def_retrieval_from_func_call;
        using MatchedFunctionData = std::pair<FunctionDefinition::Ref, GenericSubstitutionRule::Set::Ref>;

        void maybe_register_as_main_function(
            const FunctionDefinition::Ref& function_definition,
            const std::string& package_name
        );

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
        std::list<FunctionDefinition::Ref> function_definitions;
        std::unordered_map<std::string, FunctionDefinition::OverloadSet> function_definitions_overload_sets;
        std::unordered_map<std::string, FunctionDefinition::Ref> fast_retrieve_cache;
        std::list<std::pair<FunctionDefinition::Ref, std::string>> main_function_definitions_and_their_packages;

        TypeDefinitionsRegister& type_definitions_register;
        ProjectFileStructure& project_file_structure;
};