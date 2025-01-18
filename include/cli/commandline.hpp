//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <vector>
#include <string>
#include <optional>

#include "cli/file_extensions.hpp"

class CommandLineController {

    public: 
        CommandLineController(int argc, char** argv);

        enum class SubCommand {
            unspecified, 
            version, 
            help, 
            compiler, 
            interpreter, 
            typechecker
        };

        void dispatch_based_on_user_input();

    protected:
        SubCommand subcommand;
        [[nodiscard]] SubCommand parse_subcommand();

        void parse_input_files();
        void parse_output_files();

        void validate_output_files();
        void validate_input_files();
        void parse_target_triple();

        void version_subcommand();
        void help_subcommand();
        void compiler_subcommand();
        void interpreter_subcommand();
        void typechecker_subcommand();

    private:        
        const int arg_counter;
        char** arg_values;
        int arg_index;

        std::optional<std::string> target_triple;
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;
};