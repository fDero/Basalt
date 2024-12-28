//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/statement_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

void StatementLLVMTranslator::translate_conditional_into_llvm(const Conditional& conditional) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_while_loop_into_llvm(const WhileLoop& while_loop) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_until_loop_into_llvm(const UntilLoop& until_loop) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_return_statement_into_llvm(const Return& return_statement) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_break_statement_into_llvm(const Break& break_statement) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_continue_statement_into_llvm(const Continue& continue_statement) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_function_call_into_llvm(const FunctionCall& function_call) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_variable_declaration_into_llvm(const VariableDeclaration& variable_declaration) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_constant_declaration_into_llvm(const ConstDeclaration& const_declaration) {
    throw InternalError("Not implemented yet");
}

void StatementLLVMTranslator::translate_assignment_into_llvm(const Assignment& assignment) {
    throw InternalError("Not implemented yet");
}