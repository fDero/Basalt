/**
 * @file commandline.hpp
 * @author Francesco De Rosa (francescodero@outlook.it)
 * @brief This file contains the definition of the CommandLineController class
 * @version 0.1
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <vector>
#include <string>

#include "misc/file_extensions.hpp"

/**
 * @brief   Used to interpret the command line arguments and to dispatch the execution to the right 
 *          component (compiler, interpreter, etc.) setting it up with the right parameters.
 * 
 * @details The CommandLineController class is used to interpret the command line arguments and to
 *          dispatch the execution to the right component (compiler, interpreter, etc.) setting it
 *          up with the right parameters. It encapsulates its own commandline arguments parser, and
 *          it performs some minor validation on the input files and output files 
 *          (e.g. checking their existence and that their name contains correct file extensions).
 * 
 * @note    Currently, the only supported command line flags are: 
 *          --compile, --output, --help, --version, -c, -o, -h, -v.
 * 
 * @see     Compiler
 * 
 */
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