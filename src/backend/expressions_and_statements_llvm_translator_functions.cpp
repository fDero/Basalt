//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_expr_function_call_into_llvm(const FunctionCall& expr) {
    throw std::runtime_error("not implemented yet");
}

void ExpressionsAndStatementsLLVMTranslator::translate_void_function_call_into_llvm(const FunctionCall& function_call) {
    throw std::runtime_error("not implemented yet");
}