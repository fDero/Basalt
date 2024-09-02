/**
 * @file preprocessor.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the PreProcessor class
 * @version 0.1
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 */

#pragma once

#include "model/program_representation.hpp"

/**
 * @brief   Used to preprocess the program representation, in order to either assert its correctness
 *          or to communicate to the user that the source code contains errors.
 * 
 * @details The PreProcessor class delegates all of its functionalities to other classes. It's just a
 *          way to simplify the API exposing fewer methods to the callerm and providing easy management
 *          of the preprocessing steps.
 * 
 * @see     ProjectFileStructure
 * @see     TypeDefinitionsRegister
 * @see     FunctionOverloadsRegister
 * @see     OverloadingResolutionEngine
 */
class PreProcessor {

    public:
        PreProcessor(
            ProgramRepresentation& program_representation
        );

        void preprocess_packages_typename_conflicts();
        void preprocess_type_definitions();
        void preprocess_function_definitions();
    
    private:
        ProjectFileStructure& project_file_structure;
        TypeDefinitionsRegister& type_definitions_register;
        FunctionOverloadsRegister& function_overloads_register;
        OverloadingResolutionEngine& overloading_resolution_engine;
};