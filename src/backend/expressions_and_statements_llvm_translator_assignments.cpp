//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"

void ExpressionsAndStatementsLLVMTranslator::translate_variable_declaration_into_llvm(const VariableDeclaration& variable_declaration) {
    scope_context.store_local_variable(variable_declaration);
    llvm::Type* llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(variable_declaration.typesignature);
    std::string llvm_identifier_name = scope_context.resolve_object_unique_id(variable_declaration.identifier_name);
    builder.CreateAlloca(llvm_type, nullptr, llvm_identifier_name);
    throw std::runtime_error("not yet fully implemented");
}

void ExpressionsAndStatementsLLVMTranslator::translate_constant_declaration_into_llvm(const ConstDeclaration& const_declaration) {
    scope_context.store_local_constant(const_declaration);
    llvm::Type* llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(const_declaration.typesignature);
    std::string llvm_identifier_name = scope_context.resolve_object_unique_id(const_declaration.identifier_name);
    builder.CreateAlloca(llvm_type, nullptr, llvm_identifier_name);
    throw std::runtime_error("not yet fully implemented"); 
}

void ExpressionsAndStatementsLLVMTranslator::translate_assignment_into_llvm(const Assignment& assignment) {
    throw std::runtime_error("not implemented yet");
}