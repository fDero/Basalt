//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <vector>
#include <string>

#include "misc/file_extensions.hpp"

class CommandLineController {

    public: 
        CommandLineController(int argc, char** argv);
        void dispatch_based_on_user_input();

        enum class Mode {
            unspecified, version, help,
            compiler, interpreter, debugger
        };

    private:
        Mode mode;

        void compiler_flag();
        void interpreter_flag();
        void debugger_flag();
        void output_flag();
        void version_flag();
        void help_flag();

        void instantiation_and_run_compiler();
        void instantiation_and_run_interpreter();
        void instantiation_and_run_debugger();
        void display_commandline_version();
        void display_commandline_help();


    private:
        const int arg_counter;
        char** arg_values;
        int arg_index;

        enum class Flag {
            unspecified, version, help, output,
            compiler, interpreter, debugger
        };

        Flag detect_flag();
        void validate_output_files();
        void validate_input_files();

        std::vector<std::string> inputs;
        std::vector<std::string> outputs;
};