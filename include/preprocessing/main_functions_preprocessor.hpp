//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once 

#include "core/program_representation.hpp"

class MainFunctionsPreprocessor {
    
    public:
        MainFunctionsPreprocessor(ProgramRepresentation& program_representation);

        void preprocess_all_main_functions();

        void preprocess_main_function(
            const FunctionDefinition::Ref& main_function_definition,
            const std::string& package_name
        );

    private:
        ProgramRepresentation& program_representation;
};