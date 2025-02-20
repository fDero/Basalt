//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/commandline_errors.hpp"
using CompilationError::Kind::CommandLineError;

void throw_invalid_output_file_format(const std::string& filename) {
    CompilationError::raise<CommandLineError>( 
        "file " + filename + " has an unrecognized/unsupported extension"
        "(the only supported output formats are .ll, .llvm, .asm, .s, .obj, .o)" 
    );
}

void throw_invalid_input_file_format(const std::string& filename) {
    CompilationError::raise<CommandLineError>( 
        "file " + filename + " specified as an input file "
        "(input files must have the .basalt file extension)"
    );
}

void throw_unrecognized_flag(char** argv, int i) {
    CompilationError::raise<CommandLineError>( 
        "unrecognized flag: " + std::string(argv[i]) 
    );
}

void throw_basalt_used_as_output(const std::string& filename) {
    CompilationError::raise<CommandLineError>( 
        "file " + filename + " specified as an output file "
        "(basalt files can only be used as input)"
    );
}

void throw_unspecified_commandline_subcommand() {
    CompilationError::raise<CommandLineError>( 
        "unspecified mode: don't know what to do "
        "(to specify a mode please use -a --flag)" 
    );
}

void throw_unrecognized_file_extension(const std::string& filename) {
    CompilationError::raise<CommandLineError>( 
        "unrecognized file extension for file: " + filename
    );
}