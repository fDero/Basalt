
#include "toolchain/file_extensions.hpp"
#include "toolchain/compiler.hpp"
#include <iostream>

Compiler::Compiler(const std::vector<std::string>& source_files, const std::vector<std::string>& output_files) 
    : project_file_structure()
    , type_definitions_register(project_file_structure)
    , function_overloads_register(project_file_structure)
    , overloading_resolution_engine(function_overloads_register, type_definitions_register, project_file_structure)
    , preprocessor(project_file_structure, type_definitions_register, function_overloads_register, overloading_resolution_engine)
    , output_files(output_files)
{
    for (const std::string& input_file_name : source_files) {
        Tokenizer tokenizer(input_file_name);
        Parser parser(tokenizer.tokenize());
        FileRepresentation file_representation = parser.parse_everything();
        project_file_structure.store_file_representation(file_representation);
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