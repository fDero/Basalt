//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <iostream>

#include "misc/file_extensions.hpp"
#include "frontend/parser.hpp"
#include "frontend/tokenizer.hpp"
#include "backend/finalizer.hpp"

Finalizer::Finalizer(ProgramRepresentation& program_representation) 
    : Finalizer(
        program_representation, 
        llvm::sys::getDefaultTargetTriple()
    )
{}

Finalizer::Finalizer(
    ProgramRepresentation& input_program_representation,
    const std::string& target_triple
) 
    : program_representation(input_program_representation)
    , target_triple(target_triple)
    , llvm_context()
    , llvm_module("basalt_program", llvm_context)
    , llvm_builder(llvm_context)
    , type_definitions_llvm_translator(
        program_representation, 
        llvm_context, 
        llvm_module
    )
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();  
    std::string error;
    const llvm::Target* target(llvm::TargetRegistry::lookupTarget(target_triple, error));
    //ensure no error
    llvm_target_machine = target->createTargetMachine(
        target_triple,
        "generic", 
        "",
        llvm::TargetOptions(), 
        llvm::Optional<llvm::Reloc::Model>()
    );
    llvm_module.setDataLayout(llvm_target_machine->createDataLayout());
}

void Finalizer::generate_llvm_ir() {
    std::runtime_error("not implemented");
}

void Finalizer::emit_llvm_ir(const std::string& output_file_name) {
    std::cout << "Emitting LLVM..." << std::endl;
	std::error_code error_code;
	llvm::raw_fd_ostream output_file (output_file_name, error_code);
	llvm_module.print(output_file, nullptr);
	llvm_module.print(llvm::outs(), nullptr);
}

void Finalizer::emit_object_file(const std::string& output_file_name) {
    std::error_code error_code;
    llvm::raw_fd_ostream output_file(
        output_file_name, 
        error_code, 
        llvm::sys::fs::OpenFlags::OF_None
    );
    //ensure no error
    llvm::legacy::PassManager pass_manager;
    auto file_type = llvm::CodeGenFileType::CGFT_ObjectFile;

    /*bool compilation_pass_added_successfully = !*/
    llvm_target_machine->addPassesToEmitFile(pass_manager, output_file, nullptr, file_type);
    //ensure no error
    
    pass_manager.run(llvm_module);
    std::cout << "Binary emitted to " << output_file_name << std::endl;
    std::cout << "(target: " << target_triple << ")" << std::endl;
}