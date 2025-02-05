//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <iostream>

#include "frontend/tokenizer.hpp"
#include "cli/commandline.hpp"
#include "frontend/parser.hpp"
#include "preprocessing/preprocessor.hpp"
#include "backend/finalizer.hpp"
#include "errors/commandline_errors.hpp"

void CommandLineController::typechecker_subcommand() {
    ensure_lack_of_output_files(outputs);
    ensure_lack_of_target_triple(target_triple);
    validate_input_files();
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
}

void CommandLineController::compiler_subcommand() {
    validate_output_files();
    validate_input_files();
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
    Finalizer finalizer(program_representation, target_triple);
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
    using CompilationError::Kind::CommandLineError;
    CompilationError::raise<CommandLineError>(
        "`run` subcommand not yet implemented"
    );
}