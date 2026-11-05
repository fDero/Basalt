//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>

#include "errors/commandline_errors.hpp"
#include "errors/compilation_error.hpp"


void throw_invalid_output_file_format(const std::string& filename) {
    throw CommandLineError(
        "file " + filename + " has an unrecognized/unsupported extension"
        "(the only supported output formats are .ll, .llvm, .asm, .s, .obj, .o)"
    );
}

void throw_invalid_input_file_format(const std::string& filename) {
    throw CommandLineError(
        "file " + filename + " specified as an input file "
        "(input files must have the .basalt file extension)"
    );
}

void throw_unrecognized_flag(char** argv, int i) {
    throw CommandLineError(
        "unrecognized flag: " + std::string(argv[i])
    );
}

void throw_basalt_used_as_output(const std::string& filename) {
    throw CommandLineError(
        "file " + filename + " specified as an output file "
        "(basalt files can only be used as input)"
    );
}

void throw_unspecified_commandline_subcommand() {
    throw CommandLineError(
        "unspecified mode: don't know what to do "
        "(to specify a mode please use -a --flag)"
    );
}

void throw_unrecognized_file_extension(const std::string& filename) {
    throw CommandLineError(
        "unrecognized file extension for file: " + filename
    );
}

void throw_program_execution_failure(const std::string& reason) {
    throw CommandLineError(
        "unable to run the program: " + reason
    );
}
