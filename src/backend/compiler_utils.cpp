//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <iostream>

#include "misc/file_extensions.hpp"
#include "frontend/parser.hpp"
#include "frontend/tokenizer.hpp"
#include "backend/compiler.hpp"

Compiler::Compiler(const ProjectFileStructure& project_file_structure) 
    : program_representation(project_file_structure)
    , preprocessor(program_representation)
    , output_files(output_files)
    , llvm_context()
    , llvm_module("basalt_program", llvm_context)
    , llvm_builder(llvm_context)
{
    //for (const std::string& input_file_name : source_files) {
    //    Tokenizer tokenizer(input_file_name);
    //    Parser parser(tokenizer.tokenize());
    //    FileRepresentation file_representation = parser.parse_everything();
    //    program_representation.project_file_structure.store_file_representation(file_representation);
    //    for (const FunctionDefinition& function_definition : file_representation.func_defs) {
    //        program_representation.function_overloads_register.store_function_definition(function_definition);
    //    }
    //    for (const TypeDefinition& type_definition : file_representation.type_defs) {
    //        program_representation.type_definitions_register.store_type_definition(type_definition);
    //    }
    //}        
}

void Compiler::perform_static_analysis() {
    preprocessor.preprocess_packages_typename_conflicts();
    preprocessor.preprocess_type_definitions();
    preprocessor.preprocess_function_definitions();
}

void Compiler::perform_code_generation() {
    std::cout << "Code generation is not implemented yet" << std::endl;
    std::cout << "check for updates at https://www.github.com/fDero/Basalt\n\n";
}

void Compiler::output_the_required_files() {
    std::cout << "No file can be generated yet" << std::endl;
    std::cout << "check for updates at https://www.github.com/fDero/Basalt\n\n";
}