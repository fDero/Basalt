
#include "toolchain/file_extensions.hpp"
#include "toolchain/compiler.hpp"
#include <iostream>

Compiler::Compiler(const std::vector<std::string>& source_files, const std::vector<std::string>& output_files) 
    : program_representation()
    , preprocessor(program_representation)
    , output_files(output_files)
{
    for (const std::string& input_file_name : source_files) {
        Tokenizer tokenizer(input_file_name);
        Parser parser(tokenizer.tokenize());
        FileRepresentation file_representation = parser.parse_everything();
        program_representation.project_file_structure.store_file_representation(file_representation);
        for (const FunctionDefinition& function_definition : file_representation.func_defs) {
            program_representation.function_overloads_register.store_function_definition(function_definition);
        }
        for (const TypeDefinition& type_definition : file_representation.type_defs) {
            program_representation.type_definitions_register.store_type_definition(type_definition);
        }
    }        
}

void Compiler::perform_static_analysis() {
    preprocessor.preprocess_packages_typename_conflicts();
    preprocessor.preprocess_type_definitions();
    // maybe add a separate function-definitions preprocessing step??
}

void Compiler::perform_code_generation() {
    std::cout << "Code generation is not implemented yet" << std::endl;
    std::cout << "check for updates at https://www.github.com/fDero/Basalt\n\n";
}

void Compiler::output_the_required_files() {
    std::cout << "No file can be generated yet" << std::endl;
    std::cout << "check for updates at https://www.github.com/fDero/Basalt\n\n";
}