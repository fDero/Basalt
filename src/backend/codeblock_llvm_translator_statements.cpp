//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/codeblock_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

void CodeBlockLLVMTranslator::translate_conditional_into_llvm(const Conditional& conditional) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_while_loop_into_llvm(const WhileLoop& while_loop) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_until_loop_into_llvm(const UntilLoop& until_loop) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_return_statement_into_llvm(const Return& return_statement) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_break_statement_into_llvm(const Break& break_statement) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_continue_statement_into_llvm(const Continue& continue_statement) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_void_function_call_into_llvm(const FunctionCall& function_call) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_variable_declaration_into_llvm(const VariableDeclaration& variable_declaration) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_constant_declaration_into_llvm(const ConstDeclaration& const_declaration) {
    throw InternalError("Not implemented yet");
}

void CodeBlockLLVMTranslator::translate_assignment_into_llvm(const Assignment& assignment) {
    throw InternalError("Not implemented yet");
}