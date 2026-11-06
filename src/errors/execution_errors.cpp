//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>
#include <utility>

#include <llvm/IR/Function.h>
#include <llvm/Support/Error.h>

#include "errors/compilation_error.hpp"
#include "errors/execution_errors.hpp"


void throw_program_execution_failure(const std::string& reason) {
    throw CommandLineError(
        "unable to run the program: " + reason
    );
}

void ensure_jit_operation_succeeded(llvm::Error error) {
    if (error) {
        throw_program_execution_failure(llvm::toString(std::move(error)));
    }
}

void ensure_llvm_module_has_main_function(const llvm::Function* main_function) {
    if (main_function == nullptr) {
        throw_program_execution_failure("the program has no `main` function to run");
    }
}
