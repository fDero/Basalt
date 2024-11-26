//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <iostream>
#include <cassert>

#include "frontend/tokenizer.hpp"
#include "cli/commandline.hpp"
#include "frontend/parser.hpp"
#include "preprocessing/preprocessor.hpp"
#include "backend/finalizer.hpp"
#include "errors/commandline_errors.hpp"

void CommandLineController::dispatch_based_on_user_input() {
    using Mode = CommandLineController::Mode; 
    switch(mode) {
        break; case Mode::compiler:    instantiation_and_run_compiler();
        break; case Mode::interpreter: instantiation_and_run_interpreter();
        break; case Mode::debugger:    instantiation_and_run_debugger();
        break; case Mode::help:        display_commandline_help();
        break; case Mode::version:     display_commandline_version();
        break; case Mode::unspecified: throw_unspecified_commandline_mode();
    }
}

void CommandLineController::instantiation_and_run_compiler() {
    avoid_lack_of_input_files(inputs);
    avoid_lack_of_output_files(outputs);
    avoid_duplicate_input_files(inputs);
    std::vector<FileRepresentation> file_representations;
    for (const std::string& input_file : inputs) {
        Tokenizer tokenizer(input_file);
        TokenizedFile tokenized_file = tokenizer.tokenize();
        Parser parser(tokenized_file);
        FileRepresentation file_representation = parser.parse_everything();
        file_representations.push_back(file_representation);
    }
    ProjectFileStructure project_file_structure(file_representations);
    ProgramRepresentation program_representation(project_file_structure);
    PreProcessor preprocessor(program_representation);
    preprocessor.preprocess_packages_typename_conflicts();
    preprocessor.preprocess_type_definitions();
    preprocessor.preprocess_function_definitions();
    Finalizer finalizer(program_representation);
    finalizer.generate_llvm_ir();
    finalizer.emit_llvm_ir(outputs.front() + ".ll");
    finalizer.emit_object_file(outputs.front() + ".o");
}

void CommandLineController::instantiation_and_run_interpreter() {
    std::cout << "the basalt interpreter is not yet implemented\n";
    std::cout << "check for updates at https://www.github.com/fDero/Basalt\n\n";
}

void CommandLineController::instantiation_and_run_debugger() {
    std::cout << "the basalt debugger is not yet implemented\n";
    std::cout << "check for updates at https://www.github.com/fDero/Basalt\n\n";
}

void CommandLineController::display_commandline_version() {
    std::cout << "version: pre-release\n";
    std::cout << "check for updates at https://www.github.com/fDero/Basalt\n\n";
}

void CommandLineController::display_commandline_help() {
    std::cout << "compile basalt code: \n\t basalt -c <input files> -o <output files>\n\n";
    std::cout << "run code directly: \n\t basalt -r <input files>\n\n";
    std::cout << "debug your code: \n\t basalt -d <input files>\n\n";
    std::cout << "show current version: \n\t basalt -v\n\n";

    std::cout << "for non trivial tasks use the -j flag to specify the number of threads\n";
    std::cout << "you want to use for parallel operations (single thread when non specified)\n";
    std::cout << "example:\n\t basalt -c file1.basalt file2.basalt file3.basalt -o binary.elf -j 3 \n\n";

    std::cout << ">> every input file must be saved as .basalt\n";
    std::cout << ">> two different output files can't share the same file extension\n";
    std::cout << ">> the only supported output formats are .nasm, .elf, .exe\n";
    
    std::cout << "see the documentation for an in-depth explaination of the basalt toolchain\n";
    std::cout << "check for updates at https://www.github.com/fDero/Basalt\n\n";
}