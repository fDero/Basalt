//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "core/project_file_structure.hpp"
#include "language/definitions.hpp"
#include "language/functions.hpp"
#include "core/caching_aware_register.hpp"

class FunctionOverloadsRegister : public CachingAwareRegister {

    public:
        FunctionOverloadsRegister(ProjectFileStructure& project_file_structure);

        void store_function_definition(
            const FunctionDefinition& function_definition
        );

        void foreach_function_definition(
            std::function<void(FunctionDefinition::Ref)> visitor
        );
        
        [[nodiscard]] std::vector<std::string> retrieve_overload_sets_ids(
            const FunctionCall& function_call
        );

        [[nodiscard]] std::vector<FunctionDefinition::Ref>& retrieve_specific_overload_set(
            const std::string& overload_set_id
        );

    protected:
        using CachingAwareRegister::get_function_definition_overload_set_id;
        using CachingAwareRegister::get_generics_unaware_function_definition_overload_set_id;
        using CachingAwareRegister::get_function_call_overload_set_id;
        using CachingAwareRegister::get_overload_set_default_search_key;

    private:
        ProjectFileStructure& project_file_structure;
        std::list<FunctionDefinition::Ref> function_definitions;
        std::unordered_map<std::string, FunctionDefinition::OverloadSet> function_definitions_overload_sets;
};