
#pragma once

#include "model/program_representation.hpp"

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