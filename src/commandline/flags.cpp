
#include "errors/commandline_errors.hpp"
#include "toolchain/tokenizer.hpp"
#include "toolchain/commandline.hpp"
#include <iostream>

void CommandLineController::compiler_flag(){
    while (++arg_index < arg_counter && arg_values[arg_index][0] != '-')
        inputs.push_back(arg_values[arg_index]);
    avoid_conflicting_commandline_flags(mode, Mode::compiler);
    mode = Mode::compiler;
}

void CommandLineController::interpreter_flag(){
    while (++arg_index < arg_counter && arg_values[arg_index][0] != '-') 
        inputs.push_back(arg_values[arg_index]);
    avoid_conflicting_commandline_flags(mode, Mode::interpreter);
    mode = Mode::interpreter;
}

void CommandLineController::debugger_flag(){
    while (++arg_index < arg_counter && arg_values[arg_index][0] != '-')
        inputs.push_back(arg_values[arg_index]);
    avoid_conflicting_commandline_flags(mode, Mode::debugger);
    mode = Mode::debugger;
}

void CommandLineController::output_flag(){
    while (++arg_index < arg_counter && arg_values[arg_index][0] != '-')
        outputs.push_back(arg_values[arg_index]);
    avoid_lack_of_output_files(outputs);
}

void CommandLineController::version_flag(){
    ensure_version_flag_is_the_only_one(arg_index, arg_counter, inputs, outputs, mode);
    ++arg_index;
    mode = Mode::version;
}

void CommandLineController::help_flag(){
    ensure_help_flag_is_the_only_one(arg_index, arg_counter, inputs, outputs, mode);
    ++arg_index;
    mode = Mode::help;
}