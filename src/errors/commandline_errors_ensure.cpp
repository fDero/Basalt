
#include "errors/commandline_errors.hpp"
#include <algorithm>

void avoid_conflicting_commandline_flags(
    CommandLineController::Mode previous, 
    CommandLineController::Mode current
){
    if (previous != CommandLineController::Mode::unspecified){
        throw CommandLineError {
            "conflicting compiler flags: the flags -v -h -r -c -d are mutually exclusive "
            "(they cannot be used togheter)"
        };
    }
}

void avoid_lack_of_input_files(const std::vector<std::string>& input_files){
    if (input_files.empty()){
        throw CommandLineError {
            "no input files specified, don't know what to do \n"
            "(input files are supposed to be specified right after the -c/-r/-d flag \n"
            "and tehir name should end with the .basalt file extension)"
        };
    }
}

void avoid_lack_of_output_files(const std::vector<std::string>& output_files) {
    if (output_files.empty()){
        throw CommandLineError {
            "no output files specified, don't know what to do \n"
            "(output files are supposed to be specified right after the -o flag \n"
            "and should have one of these extensions: json,xml,asm,exe,elf)"
        };
    }
}

void avoid_duplicate_input_files(std::vector<std::string>& input_files){
    std::sort(input_files.begin(), input_files.end());
    if (std::unique(input_files.begin(), input_files.end()) != input_files.end()){
        throw CommandLineError {
            "duplicate input files detected! \n"
            "(input files must be specified only one single time)"
        };
    }
}

void avoid_duplicate_output_file_extensions(
    const std::vector<CommandLineController::FileExtension>& already_encountered, 
    const CommandLineController::FileExtension file_ext
){
    if (std::find(already_encountered.begin(), already_encountered.end(), file_ext) != already_encountered.end()){
        throw CommandLineError {
            "duplicate output file extension detected! \n"
            "(no pair output files should have the same file extension)"
        };
    }
}

void enure_selected_mode_is_not_trivial_for_thread_set(CommandLineController::Mode mode){
    bool trivial = (mode == CommandLineController::Mode::version);
    trivial &= (mode == CommandLineController::Mode::help);
    if (trivial){
        throw CommandLineError {
            "you can only specify the number of threads if you are doing"
            "a non trivial task (e.g compiling, running, debugging)"
        };
    }
}

void ensure_threads_are_a_reasonable_number(int threads){
    if (threads < 1 || threads > 100){
        throw CommandLineError {
            "you can only specify a positive non-zero integer number "
            "of threads not greater then 100"
        };
    }
}

void ensure_threads_are_not_already_set(bool threads_configured_manually){
    if (threads_configured_manually){
        throw CommandLineError {
            "conflicting specifications regarding the thread count for "
            "parallel code processing"
        };
    }
}