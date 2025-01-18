//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/commandline_errors.hpp"
#include "cli/commandline.hpp"

#include <iostream>

CommandLineController::CommandLineController(int argc, char** argv)
    : arg_counter(argc), arg_values(argv), arg_index(1)
{
    subcommand = parse_subcommand();
    arg_index = 2;
    while(arg_index < arg_counter) {
        auto text = std::string(arg_values[arg_index]);
        if (text == "-o" || text == "--output") {
            parse_output_files();
            continue;
        }
        if (text == "-i" || text == "--input") {
            parse_input_files();
            continue;
        }
        if (text == "-t" || text == "--target") {
            parse_target_triple();
            continue;
        }
        throw_unrecognized_flag(arg_values, arg_index);
    }
}

CommandLineController::SubCommand CommandLineController::parse_subcommand() {
    if (arg_counter < 2) {
        throw_unspecified_commandline_subcommand();
    }
    auto text = std::string(arg_values[1]);
    if (text == "version")   return SubCommand::version;
    if (text == "help")      return SubCommand::help;
    if (text == "compile")   return SubCommand::compiler;
    if (text == "typecheck") return SubCommand::typechecker;
    if (text == "run")       return SubCommand::interpreter;
    throw_unspecified_commandline_subcommand();
}

void CommandLineController::dispatch_based_on_user_input() {
    switch (subcommand) {
        break; case SubCommand::version:     version_subcommand();
        break; case SubCommand::help:        help_subcommand();
        break; case SubCommand::compiler:    compiler_subcommand();
        break; case SubCommand::interpreter: interpreter_subcommand();
        break; case SubCommand::typechecker: typechecker_subcommand();
        break; case SubCommand::unspecified: throw_unspecified_commandline_subcommand();
    }
}

void CommandLineController::parse_target_triple() {
    if (++arg_index < arg_counter) {
        target_triple = arg_values[arg_index++];
    } 
    avoid_lack_of_target_triple(target_triple);
}

void CommandLineController::parse_input_files() {
    while (++arg_index < arg_counter && arg_values[arg_index][0] != '-') {
        inputs.emplace_back(arg_values[arg_index]);
    }
}

void CommandLineController::parse_output_files() {
    while (++arg_index < arg_counter && arg_values[arg_index][0] != '-') {
        outputs.emplace_back(arg_values[arg_index]);
    }
}

void CommandLineController::validate_input_files() {
    avoid_lack_of_input_files(inputs);
    ensure_input_files_exist(inputs);
    avoid_duplicate_input_files(inputs);
    for(const std::string& in : inputs) {
        if (extract_file_extension(in) != FileExtension::basalt) { 
            throw_invalid_input_file_format(in);
        }
    }
}

void CommandLineController::validate_output_files() {
    avoid_lack_of_output_files(outputs);
    std::vector<FileExtension> encountered_extensions;
    for(const std::string& out : outputs) {
        FileExtension file_ext = extract_file_extension(out);
        encountered_extensions.push_back(file_ext);
        if (
            file_ext != FileExtension::llvm  &&
            file_ext != FileExtension::object &&
            file_ext != FileExtension::assembly
        ) {
            throw_invalid_output_file_format(out);
        }
    }
    avoid_duplicate_output_file_extensions(encountered_extensions);
}