//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/commandline_errors.hpp"

void ensure_input_files_exist(std::vector<std::string>& input_files) {
    for (const std::string& input_file_name : input_files) {
        std::fstream fstream_file_reader(input_file_name, std::ios::in);
        ensure_source_file_is_open(fstream_file_reader, input_file_name);
        fstream_file_reader.close();
    }
}

void ensure_lack_of_target_triple(const std::optional<std::string>& target_triple) {
    if (target_triple.has_value()) {
        throw CommandLineError {
            "target triple was given but it wasn't needed, \n"
            "(please run `basalt help` for more info)"
        };
    }
}

void ensure_lack_of_output_files(const std::vector<std::string>& output_files) {
    if (!output_files.empty()) {
        throw CommandLineError {
            "output files were given but they weren't needed, \n"
            "(please run `basalt help` for more info)"
        };
    }
}

void ensure_lack_of_input_files(const std::vector<std::string>& input_files) {
    if (!input_files.empty()) {
        throw CommandLineError {
            "input files were given but they weren't needed,  \n"
            "(please run `basalt help` for more info) \n"
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

void ensure_valid_target_triple(const std::string& target_triple, const std::string& error) {
    if (!error.empty()) {
        throw CommandLineError {
            "target triple " + target_triple + " is invalid:\n" + error
        };
    }
}