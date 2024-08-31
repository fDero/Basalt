
#include "model/program_representation.hpp"

ProgramRepresentation::ProgramRepresentation() 
    : project_file_structure()
    , type_definitions_register(project_file_structure)
    , function_overloads_register(project_file_structure)
    , overloading_resolution_engine(
        function_overloads_register, 
        type_definitions_register, 
        project_file_structure
    )
{}