//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/project_file_structure.hpp"
#include "core/type_definitions_register.hpp"
#include "core/function_overloads_register.hpp"
#include "core/overloading_resolution_engine.hpp"
#include "core/common_feature_adoption_plan_generation_engine.hpp"
#include "core/scope_context.hpp"

class ProgramRepresentation {
    
    public:
        ProgramRepresentation(const ProjectFileStructure& project_file_structure);

        [[nodiscard]] std::optional<TypeSignature> resolve_expression_type(
            const Expression& function_call, 
            ScopeContext& arg_types
        );

        [[nodiscard]] std::list<FileRepresentation>& get_files_by_package(const std::string& package_name);

        [[nodiscard]] TypeDefinition retrieve_type_definition(const CustomType& type_signature);
        [[nodiscard]] TypeSignature unalias_type(const TypeSignature& type_signature);

        [[nodiscard]] std::string get_fully_qualified_typesignature_name(const TypeSignature& typesignature);
        [[nodiscard]] std::string get_fully_qualified_typedefinition_name(const TypeDefinition& type_definition);
    
        void foreach_type_definition(std::function<void(const TypeDefinition&)> visitor);
        void foreach_function_definition(std::function<void(const FunctionDefinition::Ref&)> visitor);
        void verify_that_the_type_exists(const TypeSignature& type_signature);
        void foreach_package(const std::function<void(const std::string&)>& func);

        [[nodiscard]] bool validate_assignment(
            const std::optional<TypeSignature>& target,
            const std::optional<TypeSignature>& source
        );

    private:
        ProjectFileStructure project_file_structure;
        TypeDefinitionsRegister type_definitions_register;
        FunctionOverloadsRegister function_overloads_register;
        OverloadingResolutionEngine overloading_resolution_engine;
        CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generation_engine;
};