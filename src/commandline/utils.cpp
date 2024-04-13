
#include "errors/commandline_errors.hpp"
#include "toolchain/tokenizer.hpp"
#include "toolchain/commandline.hpp"

CommandLineController::CommandLineController(int argc, char** argv)
    : arg_counter(argc), arg_values(argv), arg_index(1)
{
    mode = Mode::unspecified;
    while (arg_index < arg_counter){
        switch (detect_flag()){
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
    ensure_existence_of_input_files();
}

void CommandLineController::validate_input_files(){
    for(const std::string& in : inputs) {
        if (file_extension(in) != FileExtension::basalt) { 
            throw_invalid_input_file_format(in);
        }
    }
}

void CommandLineController::validate_output_files(){
    for(const std::string& out : outputs) {
        if (
            file_extension(out) != FileExtension::nasm &&
            file_extension(out) != FileExtension::elf  &&
            file_extension(out) != FileExtension::exe
        ) 
        throw_invalid_output_file_format(out);
    }
}

void CommandLineController::ensure_existence_of_input_files(){
    for (const std::string& input_file_name : inputs){
        std::fstream fstream_file_reader(input_file_name, std::ios::in);
        ensure_source_file_is_open(fstream_file_reader, input_file_name);
        fstream_file_reader.close();
    }
}

bool CommandLineController::check_for_specific_file_extension(const std::string& filename, const std::string& extension){
    const int size = filename.size();
    const int extension_size = extension.size();
    const int prefix_size = size - extension_size;
    return (filename.substr(prefix_size, extension_size) == extension);
}

CommandLineController::Flag CommandLineController::detect_flag(){
    std::string text = std::string(arg_values[arg_index]);
    if (text == "-h" || text == "--help") return Flag::help;
    if (text == "-v" || text == "--version") return Flag::version;
    if (text == "-d" || text == "--debug") return Flag::debugger;
    if (text == "-c" || text == "--compile") return Flag::compiler;
    if (text == "-r" || text == "--run") return Flag::interpreter;
    if (text == "-o" || text == "--output") return Flag::output;
    return Flag::unspecified;
}

CommandLineController::FileExtension CommandLineController::file_extension(const std::string& filename){
    if (check_for_specific_file_extension(filename, ".basalt")) return FileExtension::basalt;
    if (check_for_specific_file_extension(filename, ".bt"))     return FileExtension::basalt;
    if (check_for_specific_file_extension(filename, ".elf"))    return FileExtension::elf;
    if (check_for_specific_file_extension(filename, ".nasm"))   return FileExtension::nasm;
    if (check_for_specific_file_extension(filename, ".exe"))    return FileExtension::exe;
    throw_unrecognized_file_extension(filename);
}