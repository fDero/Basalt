//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <iostream>

#include "backend/finalizer.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"

void Finalizer::generate_llvm_ir() {
    std::cout << "Generating LLVM IR..." << std::endl;
    program_representation.foreach_function_definition(
        [&](const FunctionDefinition::Ref& function_definition) {
            if (function_definition->template_generics_names.empty()) {
                CallableCodeBlock callable_code_block(function_definition, program_representation);
                callable_codeblocks_llvm_translator.translate_callable_code_block_to_llvm(callable_code_block);
            }
        }
    );
}

void Finalizer::emit_llvm_ir(const std::string& output_file_name) {
    std::cout << "Emitting LLVM..." << std::endl;
	std::error_code error_code;
	llvm::raw_fd_ostream output_file (output_file_name, error_code);
	llvm_module.print(output_file, nullptr);
	llvm_module.print(llvm::outs(), nullptr);
}

void Finalizer::emit_object_file(const std::string& output_file_name) {
    std::cout << "Emitting Object File..." << std::endl;
    std::error_code error_code;
    llvm::raw_fd_ostream output_file(
        output_file_name, 
        error_code, 
        llvm::sys::fs::OpenFlags::OF_None
    );

    llvm::legacy::PassManager pass_manager;
    auto file_type = llvm::CodeGenFileType::CGFT_ObjectFile;
    llvm_target_machine->addPassesToEmitFile(pass_manager, output_file, nullptr, file_type);    
    pass_manager.run(llvm_module);

    std::cout << "Result: " << output_file_name << std::endl;
    std::cout << "(target: " << target_triple << ")" << std::endl;
}

void Finalizer::emit_assembly_file(const std::string& output_file_name) {
    std::cout << "Emitting Assembly File..." << std::endl;
    std::error_code error_code;
    llvm::raw_fd_ostream output_file(
        output_file_name, 
        error_code, 
        llvm::sys::fs::OpenFlags::OF_None
    );

    llvm::legacy::PassManager pass_manager;
    auto file_type = llvm::CodeGenFileType::CGFT_AssemblyFile;
    llvm_target_machine->addPassesToEmitFile(pass_manager, output_file, nullptr, file_type);
    pass_manager.run(llvm_module);

    std::cout << "Result: " << output_file_name << std::endl;
    std::cout << "(target: " << target_triple << ")" << std::endl;
}