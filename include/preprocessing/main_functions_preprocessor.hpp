//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>

#include "language/definitions.hpp"
#include "misc/forward_declarations.hpp"


class MainFunctionsPreprocessor {

    public:
        MainFunctionsPreprocessor(ProgramRepresentation& program_representation);

        void preprocess_all_main_functions();
        void ensure_program_has_a_main_function();
        void preprocess_main_function(
            const FunctionDefinition::Ref& main_function_definition,
            const std::string& package_name
        );

    private:
        ProgramRepresentation& program_representation;
};
