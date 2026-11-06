//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Target/TargetMachine.h>

#include "core/program_representation.hpp"
#include "frontend/file_descriptor.hpp"


struct CodeGenerator {

    using LibraryFiles      = std::vector<FileDescriptor>;
    using ExecutionOutput   = int64_t;
    using CompilationTarget = llvm::TargetMachine*;
    using CompilationModule = std::unique_ptr<llvm::Module>;

    std::string target_triple;
    ProgramRepresentation program_representation;
    llvm::orc::ThreadSafeContext llvm_context;
    LibraryFiles library_files;

    CodeGenerator(
        const ProgramRepresentation& program_representation,
        std::optional<std::string> target_triple,
        const LibraryFiles& libraries
    );

    [[nodiscard]] CompilationTarget initialize_target();
    [[nodiscard]] CompilationModule initialize_module();
    [[nodiscard]] ExecutionOutput   run(CompilationModule&& module);

    void emit_llvm(llvm::Module& module, CompilationTarget target, const FileDescriptor& output_file);
    void emit_object(llvm::Module& module, CompilationTarget target, const FileDescriptor& output_file);
    void emit_assembly(llvm::Module& module, CompilationTarget target, const FileDescriptor& output_file);
};
