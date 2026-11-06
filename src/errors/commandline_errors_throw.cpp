//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>

#include "frontend/file_descriptor.hpp"
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
        "(input files must have the .basalt or .bt file extension)"
    );
}

void throw_invalid_library_file_format(const std::string& filename) {
    throw CommandLineError(
        "file " + filename + " specified as a library to link "
        "(libraries must have one of these extensions: .a .lib .so .dylib .dll)"
    );
}

void throw_unrecognized_flag(char** argv, int i) {
    throw_unrecognized_flag(std::string(argv[i]));
}

void throw_unrecognized_flag(const std::string& flag) {
    throw CommandLineError(
        "unrecognized flag: " + flag
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
        "(please run `basalt help` for more info)"
    );
}

void throw_unrecognized_file_extension(const std::string& filename) {
    throw CommandLineError(
        "unrecognized file extension for file: " + filename
    );
}

void throw_unrecognized_file_extension(const FileDescriptor& file) {
    throw_unrecognized_file_extension(file.fullpath);
}
