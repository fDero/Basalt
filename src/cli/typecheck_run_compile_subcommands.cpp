//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <iostream>
#include <string>
#include <vector>

#include "backend/finalizer.hpp"
#include "cli/commandline.hpp"
#include "cli/file_extensions.hpp"
#include "core/program_representation.hpp"
#include "core/project_file_structure.hpp"
#include "errors/commandline_errors.hpp"
#include "frontend/file_representation.hpp"
#include "frontend/parser.hpp"
#include "frontend/tokenized_file.hpp"
#include "frontend/tokenizer.hpp"
#include "preprocessing/preprocessor.hpp"


ProjectFileStructure CommandLineController::load_project_file_structure() {
    std::vector<FileRepresentation> file_representations;
    for (const std::string& input_file : inputs) {
        Tokenizer tokenizer(input_file);
        TokenizedFile tokenized_file = tokenizer.tokenize();
        Parser parser(tokenized_file);
        FileRepresentation file_representation = parser.parse_everything();
        file_representations.push_back(file_representation);
    }
    return ProjectFileStructure(file_representations);
}

void CommandLineController::preprocess_program(ProgramRepresentation& program_representation) {
    PreProcessor preprocessor(program_representation);
    preprocessor.preprocess_packages_typename_conflicts();
    preprocessor.preprocess_type_definitions();
    preprocessor.preprocess_function_definitions();
}

void CommandLineController::typechecker_subcommand() {
    ensure_lack_of_output_files(outputs);
    ensure_lack_of_target_triple(target_triple);
    validate_input_files();
    ProgramRepresentation program_representation(load_project_file_structure());
    preprocess_program(program_representation);
}

void CommandLineController::compiler_subcommand() {
    validate_output_files();
    validate_input_files();
    ProgramRepresentation program_representation(load_project_file_structure());
    preprocess_program(program_representation);
    PreProcessor(program_representation).ensure_program_has_a_main_function();
    Finalizer finalizer(program_representation, target_triple);
    std::cout << "Generating LLVM IR..." << std::endl;
    finalizer.generate_llvm_ir();
    for (const std::string& output_file : outputs) {
        switch (extract_file_extension(output_file)) {
            break; case FileExtension::llvm: finalizer.emit_llvm_ir(output_file);
            break; case FileExtension::object: finalizer.emit_object_file(output_file);
            break; case FileExtension::assembly: finalizer.emit_assembly_file(output_file);
            break; case FileExtension::basalt: throw_basalt_used_as_output(output_file);
        }
    }
}

void CommandLineController::interpreter_subcommand() {
    ensure_lack_of_output_files(outputs);
    ensure_lack_of_target_triple(target_triple);
    validate_input_files();
    ProgramRepresentation program_representation(load_project_file_structure());
    preprocess_program(program_representation);
    PreProcessor(program_representation).ensure_program_has_a_main_function();
    Finalizer finalizer(program_representation);
    finalizer.generate_llvm_ir();
    int out = finalizer.run_jit();
    std::cout << "Execution output: " << out << std::endl;
}
