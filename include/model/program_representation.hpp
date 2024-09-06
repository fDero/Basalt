
#pragma once

#include "model/project_file_structure.hpp"
#include "model/type_definitions_register.hpp"
#include "model/function_overloads_register.hpp"
#include "model/overloading_resolution_engine.hpp"

struct ProgramRepresentation {
        
    ProjectFileStructure project_file_structure;
    TypeDefinitionsRegister type_definitions_register;
    FunctionOverloadsRegister function_overloads_register;
    OverloadingResolutionEngine overloading_resolution_engine;

    ProgramRepresentation();
};