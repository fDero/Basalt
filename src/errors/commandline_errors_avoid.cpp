//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/commandline_errors.hpp"

void ensure_lack_of_output_files(const std::vector<std::string>& output_files) {
    if (!output_files.empty()) {
        throw CommandLineError {
           "output files were given but they weren't needed, \n"
            "(please keep in mind that you will get an output only \n"
            "when compiling, which means you only need to specify \n"
            "output files if you use the compilation flag)"
        };
    }
}

void ensure_source_file_is_open(const std::fstream& input_file, const std::string& file_name) {
    if (!input_file.is_open()) {
        throw CommandLineError {
            "file " + file_name + " either is missing or is inaccessible \n"
            "(please double-check the name of the file and make sure the \n"
            "path to that file is fully and correctly specified)"
        };
    }
}

void ensure_version_flag_is_the_only_one(
    int current_flag_index, int arg_counter, const std::vector<std::string>& input_files, 
    const std::vector<std::string>& output_files, const CommandLineController::Mode mode
) {
    if (
        !input_files.empty()  || !output_files.empty() || 
        current_flag_index + 1 < arg_counter || 
        mode != CommandLineController::Mode::unspecified
    ) 
    throw CommandLineError { 
        "if you use the version flag (-v / --version) that's supposed to be the only one" 
    };
}

void ensure_help_flag_is_the_only_one(
    int current_flag_index, int arg_counter, const std::vector<std::string>& input_files, 
    const std::vector<std::string>& output_files, const CommandLineController::Mode mode
) {
    if (
        !input_files.empty()  || !output_files.empty() || 
        current_flag_index + 1 < arg_counter || 
        mode != CommandLineController::Mode::unspecified
    ) 
    throw CommandLineError { 
        "if you use the help flag (-h / --help) that's supposed to be the only one" 
    };
}