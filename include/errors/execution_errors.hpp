//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <utility>

#include <llvm/IR/Function.h>
#include <llvm/Support/Error.h>


[[noreturn]] void throw_program_execution_failure(const std::string& reason);

void ensure_jit_operation_succeeded(llvm::Error error);
void ensure_llvm_module_has_main_function(const llvm::Function* main_function);

template <typename T>
[[nodiscard]] T ensure_jit_value_is_available(llvm::Expected<T> expected) {
    if (!expected) {
        throw_program_execution_failure(llvm::toString(expected.takeError()));
    }
    return std::move(*expected);
}

template <typename T>
void ensure_library_is_loaded(llvm::Expected<T>* expected, const std::string& library) {
    if (!*expected) {
        throw_program_execution_failure("unable to load library " + library +
        ": " + llvm::toString(expected->takeError()));
    }
}
