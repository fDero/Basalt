//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "misc/forward_declarations.hpp"


class PreProcessor {
    public:
        PreProcessor(ProgramRepresentation& program_representation);
        void preprocess_packages_typename_conflicts();
        void preprocess_type_definitions();
        void preprocess_function_definitions();
        void ensure_program_has_a_main_function();

    private:
        ProgramRepresentation& program_representation;
};
