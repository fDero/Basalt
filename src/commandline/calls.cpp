
#include "toolchain/tokenizer.hpp"
#include "toolchain/commandline.hpp"
#include "toolchain/parser.hpp"
#include "toolchain/preprocessor.hpp"
#include "errors/commandline_errors.hpp"
#include <iostream>
#include <cassert>

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
    std::vector<FileExtension> already_encountered_file_extensions;
    for (const std::string& out : outputs) {
        FileExtension file_ext = file_extension(out);
        avoid_duplicate_output_file_extensions(already_encountered_file_extensions, file_ext);
        already_encountered_file_extensions.push_back(file_ext);
        switch (file_ext) {
            break; case FileExtension::nasm:  /* WORK IN PROGRESS */;
            break; case FileExtension::exe:   /* WORK IN PROGRESS */;
            break; case FileExtension::elf:   /* WORK IN PROGRESS */; 
            break; case FileExtension::basalt: throw_basalt_used_as_output(out);       
        }
    }
}

void CommandLineController::instantiation_and_run_interpreter() {
    avoid_lack_of_input_files(inputs);
    avoid_duplicate_input_files(inputs);
    ensure_lack_of_output_files(outputs);
    ProjectFileStructure project_file_structure;
    for (const std::string& input_file_name : inputs) {
        Tokenizer tokenizer(input_file_name);
        Parser parser(tokenizer.tokenize());
        FileRepresentation file_representation = parser.parse_everything();
        project_file_structure.store_file_representation(file_representation);
    }
    TypeDefinitionsRegister type_definitions_register(project_file_structure);
    FunctionOverloadsRegister function_overloads_register(project_file_structure);
    OverloadingResolutionEngine overloading_resolution_engine(function_overloads_register, type_definitions_register, project_file_structure);
    PreProcessor preprocessor(project_file_structure, type_definitions_register, function_overloads_register, overloading_resolution_engine);
    preprocessor.preprocess_packages_typename_conflicts();
    preprocessor.preprocess_type_definitions();
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