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

struct ProgramRepresentation {
        
    ProjectFileStructure project_file_structure;
    TypeDefinitionsRegister type_definitions_register;
    FunctionOverloadsRegister function_overloads_register;
    OverloadingResolutionEngine overloading_resolution_engine;
    CommonFeatureAdoptionPlanGenerationEngine common_feature_adoption_plan_generation_engine;

    ProgramRepresentation();

    std::optional<TypeSignature> resolve_function_call_return_type(
        const FunctionCall& function_call, 
        const std::vector<TypeSignature>& arg_types
    );
};