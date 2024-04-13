
#include "errors/commandline_errors.hpp"

[[noreturn]] void throw_invalid_output_file_format(const std::string& filename){
    throw CommandLineError { 
        "file " + filename + " has an unrecognized/unsupported extension"
        "(the only supported output formats are json, xml, nasm, elf, exe, nasm)" 
    };
}

[[noreturn]] void throw_invalid_input_file_format(const std::string& filename){
    throw CommandLineError { 
        "file " + filename + " specified as an input file "
        "(input files must have the .basalt file extension)"
    };
}

[[noreturn]] void throw_unrecognized_flag(char** argv, int i){
    throw CommandLineError { 
        "unrecognized flag: " + std::string(argv[i]) 
    };
}

[[noreturn]] void throw_basalt_used_as_output(const std::string& filename){
    throw CommandLineError {
        "file " + filename + " specified as an output file "
        "(basalt files can only be used as input)"
    };
}

[[noreturn]] void throw_unspecified_commandline_mode(){
    throw CommandLineError { 
        "unspecified mode: don't know what to do "
        "(to specify a mode please use -a --flag)" 
    };
}

[[noreturn]] void throw_unrecognized_file_extension(const std::string& filename){
    throw CommandLineError {
        "unrecognized file extension for file: " + filename
    };
}