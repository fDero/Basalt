//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <algorithm>
#include <string>
#include <vector>

#include "frontend/file_descriptor.hpp"
#include "errors/commandline_errors.hpp"
#include "errors/compilation_error.hpp"


void avoid_lack_of_input_files(const std::vector<std::string>& input_files) {
    if (input_files.empty()) {
        throw CommandLineError(
            "no input files specified, don't know what to do \n"
            "(input files are supposed to be specified right after the -s flag \n"
            "and should have one of these extensions: .basalt .bt)"
        );
    }
}

void avoid_lack_of_output_files(const std::vector<std::string>& output_files) {
    if (output_files.empty()) {
        throw CommandLineError(
            "no output files specified, don't know what to do \n"
            "(output files are supposed to be specified right after the -o flag \n"
            "and should have one of these extensions: .ll .llvm .asm .s .obj .o)"
        );
    }
}

void avoid_duplicate_input_files(std::vector<std::string> input_files) {
    std::sort(input_files.begin(), input_files.end());
    if (std::unique(input_files.begin(), input_files.end()) != input_files.end()) {
        throw CommandLineError(
            "duplicate input files detected! \n"
            "(input files must be specified only one single time)"
        );
    }
}

void avoid_duplicate_output_file_extensions(const std::vector<FileDescriptor>& output_files) {
    std::vector<FileDescriptor::FileExtension> extensions;
    for (const FileDescriptor& file : output_files) {
        extensions.push_back(file.extension);
    }
    std::sort(extensions.begin(), extensions.end());
    if (std::unique(extensions.begin(), extensions.end()) != extensions.end()) {
        throw CommandLineError(
            "duplicate output file extension detected! \n"
            "(no pair output files should have the same file extension)"
        );
    }
}

void avoid_multiple_target_triples(const std::vector<std::string>& target_triples) {
    if (target_triples.size() > 1) {
        throw CommandLineError(
            "multiple target triples specified, don't know which one to use \n"
            "(at most one target triple can follow the -t flag)"
        );
    }
}
