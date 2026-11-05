//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <cstdint>
#include <memory>
#include <utility>

#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/Error.h>

#include "backend/finalizer.hpp"
#include "errors/commandline_errors.hpp"
#include "errors/internal_errors.hpp"


#if defined(__MINGW32__)
extern "C" void ___chkstk_ms();

static void mingw_main_stub() {}
#endif

static llvm::orc::SymbolMap host_runtime_support_symbols(llvm::orc::LLJIT& jit) {
    llvm::orc::SymbolMap symbols;
#if defined(__MINGW32__)
    symbols[jit.mangleAndIntern("__main")] = llvm::JITEvaluatedSymbol::fromPointer(&mingw_main_stub);
    symbols[jit.mangleAndIntern("___chkstk_ms")] = llvm::JITEvaluatedSymbol::fromPointer(&___chkstk_ms);
#endif
    return symbols;
}

template <typename T>
static T unwrap_or_throw(llvm::Expected<T> expected) {
    if (!expected) {
        throw_program_execution_failure(llvm::toString(expected.takeError()));
    }
    return std::move(*expected);
}

static void ensure_success(llvm::Error error) {
    if (error) {
        throw_program_execution_failure(llvm::toString(std::move(error)));
    }
}

int Finalizer::run_jit() {
    llvm::Function* main_function = llvm_module->getFunction("main");
    if (main_function == nullptr) {
        throw_internal_error("the program has no `main` function to run");
    }
    const bool main_returns_a_value = !main_function->getReturnType()->isVoidTy();
    std::unique_ptr<llvm::orc::LLJIT> jit = unwrap_or_throw(llvm::orc::LLJITBuilder().create());
    llvm_module->setDataLayout(jit->getDataLayout());
    std::unique_ptr<llvm::orc::DynamicLibrarySearchGenerator> process_symbols = unwrap_or_throw(
        llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(jit->getDataLayout().getGlobalPrefix())
    );
    jit->getMainJITDylib().addGenerator(std::move(process_symbols));
    llvm::orc::SymbolMap runtime_support_symbols = host_runtime_support_symbols(*jit);
    if (!runtime_support_symbols.empty()) {
        ensure_success(jit->getMainJITDylib().define(llvm::orc::absoluteSymbols(std::move(runtime_support_symbols))));
    }
    ensure_success(jit->addIRModule(llvm::orc::ThreadSafeModule(std::move(llvm_module), std::move(llvm_context))));
    llvm::JITEvaluatedSymbol main_symbol = unwrap_or_throw(jit->lookup("main"));
    if (main_returns_a_value) {
        auto main_entry_point = reinterpret_cast<int64_t(*)()>(main_symbol.getAddress());
        return static_cast<int>(main_entry_point());
    }
    auto main_entry_point = reinterpret_cast<void(*)()>(main_symbol.getAddress());
    main_entry_point();
    return 0;
}
