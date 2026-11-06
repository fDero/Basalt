//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cstdint>
#include <string>
#include <utility>

#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>

#include "backend/code_generator.hpp"
#include "frontend/file_descriptor.hpp"
#include "errors/execution_errors.hpp"


using LLVMReturnType = llvm::Type*;
using LibraryFiles = CodeGenerator::LibraryFiles;
using ExecutionOutput = CodeGenerator::ExecutionOutput;
using DLL = llvm::orc::DynamicLibrarySearchGenerator;
using LIB = llvm::orc::StaticLibraryDefinitionGenerator;

static void link_library(llvm::orc::LLJIT& jit, const std::string& library) {
    auto static_ext = FileDescriptor::FileExtension::static_library;
    bool is_static_lib = FileDescriptor(library).extension == static_ext;
    if (is_static_lib) {
        auto link = LIB::Load(jit.getObjLinkingLayer(), library.c_str());
        ensure_library_is_loaded(&link, library);
        jit.getMainJITDylib().addGenerator(std::move(*link));
    }
    else {
        auto link = DLL::Load(library.c_str(), jit.getDataLayout().getGlobalPrefix());
        ensure_library_is_loaded(&link, library);
        jit.getMainJITDylib().addGenerator(std::move(*link));
    }
}

ExecutionOutput CodeGenerator::run(CompilationModule&& module) {
    auto jit = ensure_jit_value_is_available(llvm::orc::LLJITBuilder().create());
    auto main_function = module->getFunction("main");
    auto return_type = main_function->getReturnType();
    ensure_llvm_module_has_main_function(main_function);
    module->setDataLayout(jit->getDataLayout());
    module->getFunction("main")->setName("__basalt_main");
    for (const FileDescriptor& library : library_files) {
        link_library(*jit, library.fullpath);
    }
    auto tsmodule = llvm::orc::ThreadSafeModule(std::move(module), llvm_context);
    ensure_jit_operation_succeeded(jit->addIRModule(std::move(tsmodule)));
    auto main_address = ensure_jit_value_is_available(jit->lookup("__basalt_main")).getAddress();
    switch (return_type != nullptr) {
        case true: return llvm::jitTargetAddressToFunction<std::int64_t(*)()>(main_address)();
        case false: llvm::jitTargetAddressToFunction<void(*)()>(main_address)();
    }
    return EXIT_SUCCESS;
}
