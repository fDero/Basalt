
#include "errors/commandline_errors.hpp"
#include "toolchain/tokenizer.hpp"
#include "toolchain/commandline.hpp"

#include <iostream>

CommandLineController::CommandLineController(int argc, char** argv)
    : arg_counter(argc), arg_values(argv), arg_index(1)
{
    mode = Mode::unspecified;
    while (arg_index < arg_counter) {
        switch (detect_flag()) {
            break; case Flag::compiler:    compiler_flag();
            break; case Flag::interpreter: interpreter_flag();
            break; case Flag::output:      output_flag();
            break; case Flag::debugger:    debugger_flag();
            break; case Flag::version:     version_flag();
            break; case Flag::help:        help_flag();
            break; case Flag::unspecified: throw_unrecognized_flag(arg_values, arg_index);
        }
    }
    validate_input_files();
    validate_output_files();
}

void CommandLineController::validate_input_files() {
    for(const std::string& in : inputs) {
        if (extract_file_extension(in) != FileExtension::basalt) { 
            throw_invalid_input_file_format(in);
        }
    }
    ensure_input_files_exist(inputs);
}

void CommandLineController::validate_output_files() {
    for(const std::string& out : outputs) {
        FileExtension file_ext = extract_file_extension(out);
        if (
            file_ext != FileExtension::ll  &&
            file_ext != FileExtension::elf &&
            file_ext != FileExtension::exe
        ) {
            throw_invalid_output_file_format(out);
        }
    }
}

CommandLineController::Flag CommandLineController::detect_flag() {
    std::string text = std::string(arg_values[arg_index]);
    if (text == "-h" || text == "--help")    return Flag::help;
    if (text == "-v" || text == "--version") return Flag::version;
    if (text == "-d" || text == "--debug")   return Flag::debugger;
    if (text == "-c" || text == "--compile") return Flag::compiler;
    if (text == "-r" || text == "--run")     return Flag::interpreter;
    if (text == "-o" || text == "--output")  return Flag::output;
    return Flag::unspecified;
}