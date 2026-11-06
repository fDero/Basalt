//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <string>

#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "errors/internal_errors.hpp"
#include "errors/internal_errors_llvm.hpp"


void assert_llvm_module_is_valid(const llvm::Module& llvm_module) {
    std::string verifier_report;
    llvm::raw_string_ostream verifier_stream(verifier_report);
    if (llvm::verifyModule(llvm_module, &verifier_stream)) {
        throw_internal_error("the generated LLVM IR is invalid: " + verifier_stream.str());
    }
}
