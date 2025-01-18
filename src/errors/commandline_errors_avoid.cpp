//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/commandline_errors.hpp"

void avoid_lack_of_input_files(const std::vector<std::string>& input_files) {
    if (input_files.empty()) {
        throw CommandLineError {
            "no input files specified, don't know what to do \n"
            "(input files are supposed to be specified right after the -i flag \n"
            "and should have one of these extensions: .basalt .bt)"
        };
    }
}

void avoid_lack_of_output_files(const std::vector<std::string>& output_files) {
    if (output_files.empty()) {
        throw CommandLineError {
            "no output files specified, don't know what to do \n"
            "(output files are supposed to be specified right after the -o flag \n"
            "and should have one of these extensions: .ll .llvm .asm .s .obj .o)"
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

void avoid_duplicate_output_file_extensions(
    std::vector<FileExtension> file_extensions
) {
    std::sort(file_extensions.begin(), file_extensions.end());
    if (std::unique(file_extensions.begin(), file_extensions.end()) != file_extensions.end()) {
        throw CommandLineError {
            "duplicate output file extension detected! \n"
            "(no pair output files should have the same file extension)"
        };
    }
}

void avoid_lack_of_target_triple(const std::optional<std::string>& target_triple) {
    if (!target_triple.has_value()) {
        throw CommandLineError {
            "no target triple specified, don't know what to do \n"
            "(target triple is supposed to be specified right after the -t flag)"
        };
    }
}