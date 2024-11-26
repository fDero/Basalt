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
    auto int32_type = llvm_builder.getInt32Ty();
	auto byte_pointer_type = llvm_builder.getInt8Ty()->getPointerTo();

    bool constexpr VARARG = true;
    bool constexpr NON_VARARG = false;

	llvm::Constant* hello_world_str = llvm::ConstantDataArray::getString(llvm_context, "Hello, World!\n", true);
	auto hello_world_var = new llvm::GlobalVariable(llvm_module, hello_world_str->getType(), true, llvm::GlobalValue::PrivateLinkage, hello_world_str);
	auto hello_world_ptr = llvm_builder.CreateConstGEP2_32(hello_world_var->getValueType(), hello_world_var, 0, 0);

	llvm::Type* printf_types[] = {byte_pointer_type};
	llvm::FunctionType* printf_type = llvm::FunctionType::get(int32_type, printf_types, VARARG);
	llvm_module.getOrInsertFunction("printf", printf_type);

	auto main_function_type = llvm::FunctionType::get(int32_type, NON_VARARG);
	auto linkage_policy = llvm::Function::ExternalLinkage;
	auto main_function = llvm::Function::Create(main_function_type, linkage_policy, "main", llvm_module);

	auto main_function_code_block = llvm::BasicBlock::Create(llvm_context, "entry", main_function);
	llvm_builder.SetInsertPoint(main_function_code_block);

	llvm_builder.CreateCall(llvm_module.getFunction("printf"), {hello_world_ptr});

	auto result = llvm_builder.getInt32(EXIT_SUCCESS);
    auto i32Result = llvm_builder.CreateIntCast(result, llvm_builder.getInt32Ty(), true);
    llvm_builder.CreateRet(i32Result);
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
    bool compilation_pass_added_successfully = !llvm_target_machine->addPassesToEmitFile(pass_manager, output_file, nullptr, file_type);
    //ensure no error
    pass_manager.run(llvm_module);
    std::cout << "Binary emitted to " << output_file_name << std::endl;
    std::cout << "(target: " << target_triple << ")" << std::endl;
}