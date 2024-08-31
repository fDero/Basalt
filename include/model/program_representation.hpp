/**
 * @file program_representation.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the ProgramRepresentation struct
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "model/project_file_structure.hpp"
#include "model/type_definitions_register.hpp"
#include "model/function_overloads_register.hpp"
#include "model/overloading_resolution_engine.hpp"

/**
 * @brief Just a container for all the data structures that represent the program
 * 
 */
struct ProgramRepresentation {
        
    ProjectFileStructure project_file_structure;
    TypeDefinitionsRegister type_definitions_register;
    FunctionOverloadsRegister function_overloads_register;
    OverloadingResolutionEngine overloading_resolution_engine;

    ProgramRepresentation();
};