//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "core/program_representation.hpp"

ProgramRepresentation::ProgramRepresentation() 
    : project_file_structure()
    , type_definitions_register(project_file_structure)
    , function_overloads_register(project_file_structure)
    , overloading_resolution_engine(
        function_overloads_register, 
        type_definitions_register, 
        project_file_structure
    )
    , common_feature_adoption_plan_generation_engine(
        overloading_resolution_engine,
        type_definitions_register
    )
{}

std::optional<TypeSignature> 
ProgramRepresentation::resolve_function_call_return_type(
    const FunctionCall& function_call, 
    const std::vector<TypeSignature>& arg_types
) {
    FunctionDefinition::Ref retrieved = 
        overloading_resolution_engine
            .retrieve_function_definition(function_call, arg_types);
    if (retrieved != nullptr) {
        return retrieved->return_type;
    }
    CommonFeatureAdoptionPlanDescriptor cfa = 
        common_feature_adoption_plan_generation_engine
            .generate_common_feature_adoption_plan(function_call, arg_types);
    return cfa.get_return_type();
}