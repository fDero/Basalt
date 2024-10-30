//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <vector>
#include <string>

#include "model/program_representation.hpp"
#include "preprocessing/preprocessor.hpp"

class Compiler {

    public:
        Compiler(
            const std::vector<std::string>& source_files, 
            const std::vector<std::string>& output_files
        );

        void perform_static_analysis();
        void perform_code_generation();
        void output_the_required_files();

    private:
        ProgramRepresentation program_representation;
        PreProcessor preprocessor;
        
        const std::vector<std::string>& output_files;
};