//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>

#include <llvm/ADT/Optional.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "core/callable_code_block.hpp"
#include "core/program_representation.hpp"
#include "backend/code_generator.hpp"
#include "errors/commandline_errors.hpp"
#include "errors/internal_errors_llvm.hpp"
#include "language/definitions.hpp"


CodeGenerator::CodeGenerator(
    const ProgramRepresentation& program_representation,
    std::optional<std::string> target_triple,
    const LibraryFiles& library_files
)
    : target_triple(target_triple.value_or(llvm::sys::getDefaultTargetTriple()))
    , program_representation(program_representation)
    , llvm_context(std::make_unique<llvm::LLVMContext>())
    , library_files(library_files)
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
}

CodeGenerator::CompilationTarget CodeGenerator::initialize_target() {
    std::string error;
    const llvm::Target* target(llvm::TargetRegistry::lookupTarget(target_triple, error));
    ensure_valid_target_triple(target_triple, error);
    const std::string cpu_kind = "generic";
    const std::string llvm_features = "";
    return target->createTargetMachine(
        target_triple,
        cpu_kind,
        llvm_features,
        llvm::TargetOptions(),
        llvm::Optional<llvm::Reloc::Model>()
    );
}

CodeGenerator::CompilationModule CodeGenerator::initialize_module() {
    llvm::LLVMContext& context = *llvm_context.getContext();
    auto module = std::make_unique<llvm::Module>("basalt_program", context);
    TypeDefinitionsLLVMTranslator type_translator(program_representation, context, *module);
    CallableCodeBlocksLLVMTranslator cblock_translator(program_representation, type_translator, context, *module);
    program_representation.foreach_function_definition(
        [&](const FunctionDefinition::Ref& function_definition) {
            if (function_definition->template_generics_names.empty()) {
                CallableCodeBlock callable_code_block(function_definition, program_representation);
                cblock_translator.translate_callable_code_block_to_llvm(callable_code_block);
            }
        }
    );
    assert_llvm_module_is_valid(*module);
    return module;
}

void CodeGenerator::emit_llvm(llvm::Module& module, CompilationTarget target, const FileDescriptor& output_file) {
    std::cout << "Emitting LLVM..." << std::endl;
    std::error_code error_code;
    const auto flags = llvm::sys::fs::OpenFlags::OF_None;
    llvm::raw_fd_ostream oss(output_file.fullpath, error_code, flags);
    module.print(oss, nullptr);
    module.print(llvm::outs(), nullptr);
}

void CodeGenerator::emit_object(llvm::Module& module, CompilationTarget target, const FileDescriptor& output_file) {
    std::cout << "Emitting Object File..." << std::endl;
    std::error_code error_code;
    const auto flags = llvm::sys::fs::OpenFlags::OF_None;
    llvm::raw_fd_ostream oss(output_file.fullpath, error_code, flags);
    module.setDataLayout(target->createDataLayout());
    module.setTargetTriple(target_triple);
    llvm::legacy::PassManager pass_manager;
    auto file_type = llvm::CodeGenFileType::CGFT_ObjectFile;
    target->addPassesToEmitFile(pass_manager, oss, nullptr, file_type);
    pass_manager.run(module);
    std::cout << "Result: " << output_file.fullpath << std::endl;
    std::cout << "(target: " << target_triple << ")" << std::endl;
}

void CodeGenerator::emit_assembly(llvm::Module& module, CompilationTarget target, const FileDescriptor& output_file) {
    std::cout << "Emitting Assembly File..." << std::endl;
    std::error_code error_code;
    const auto flags = llvm::sys::fs::OpenFlags::OF_None;
    llvm::raw_fd_ostream oss(output_file.fullpath, error_code, flags);
    module.setDataLayout(target->createDataLayout());
    module.setTargetTriple(target_triple);
    llvm::legacy::PassManager pass_manager;
    auto file_type = llvm::CodeGenFileType::CGFT_AssemblyFile;
    target->addPassesToEmitFile(pass_manager, oss, nullptr, file_type);
    pass_manager.run(module);
    std::cout << "Result: " << output_file.fullpath << std::endl;
    std::cout << "(target: " << target_triple << ")" << std::endl;
}
