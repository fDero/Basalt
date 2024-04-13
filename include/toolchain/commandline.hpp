
#pragma once
#include <vector>
#include <string>

class CommandLineController {

    public: 
        CommandLineController(int argc, char** argv);
        void dispatch_based_on_user_input();

        enum class Mode {
            unspecified, version, help,
            compiler, interpreter, debugger
        };

        enum class FileExtension {
            basalt, nasm,
            elf, exe
        };

    private:
        Mode mode;

        void compiler_flag();
        void interpreter_flag();
        void debugger_flag();
        void output_flag();
        void version_flag();
        void help_flag();

        void instanciate_and_run_compiler();
        void instanciate_and_run_interpreter();
        void instanciate_and_run_debugger();
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

        bool check_for_specific_file_extension(const std::string& filename, const std::string& extension);
        Flag detect_flag();
        FileExtension file_extension(const std::string& filename);
        void validate_output_files();
        void validate_input_files();
        void ensure_existence_of_input_files();

        std::vector<std::string> inputs;
        std::vector<std::string> outputs;
};