
#include "errors/commandline_errors.hpp"

void avoid_conflicting_commandline_flags(
    CommandLineController::Mode previous, 
    CommandLineController::Mode current
) {
    if (previous != CommandLineController::Mode::unspecified) {
        throw CommandLineError {
            "conflicting compiler flags: the flags -v -h -r -c -d are mutually exclusive "
            "(they cannot be used together)"
        };
    }
}

void avoid_lack_of_input_files(const std::vector<std::string>& input_files) {
    if (input_files.empty()) {
        throw CommandLineError {
            "no input files specified, don't know what to do \n"
            "(input files are supposed to be specified right after the -c/-r/-d flag \n"
            "and their name should end with the .basalt file extension)"
        };
    }
}

void avoid_lack_of_output_files(const std::vector<std::string>& output_files) {
    if (output_files.empty()) {
        throw CommandLineError {
            "no output files specified, don't know what to do \n"
            "(output files are supposed to be specified right after the -o flag \n"
            "and should have one of these extensions: json,xml,asm,exe,elf)"
        };
    }
}

void avoid_duplicate_input_files(std::vector<std::string>& input_files) {
    std::sort(input_files.begin(), input_files.end());
    if (std::unique(input_files.begin(), input_files.end()) != input_files.end()) {
        throw CommandLineError {
            "duplicate input files detected! \n"
            "(input files must be specified only one single time)"
        };
    }
}

void ensure_input_files_exist(std::vector<std::string>& input_files) {
    for (const std::string& input_file_name : input_files) {
        std::fstream fstream_file_reader(input_file_name, std::ios::in);
        ensure_source_file_is_open(fstream_file_reader, input_file_name);
        fstream_file_reader.close();
    }
}

void avoid_duplicate_output_file_extensions(
    const std::vector<FileExtension>& already_encountered, 
    const FileExtension file_ext
) {
    if (std::find(already_encountered.begin(), already_encountered.end(), file_ext) != already_encountered.end()) {
        throw CommandLineError {
            "duplicate output file extension detected! \n"
            "(no pair output files should have the same file extension)"
        };
    }
}