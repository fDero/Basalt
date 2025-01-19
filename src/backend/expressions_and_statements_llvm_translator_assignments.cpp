//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_manipulations_llvm_translator.hpp"

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_variable_declaration_to_llvm(
    llvm::BasicBlock* block,
    const VariableDeclaration& variable_declaration
) {
    llvm::Type* llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(variable_declaration.typesignature);
    llvm::IRBuilder<> builder(block);
    llvm::AllocaInst* alloca_inst = builder.CreateAlloca(llvm_type, nullptr);
    scope_context.store_local_variable(variable_declaration, alloca_inst);
    if (variable_declaration.initial_value.has_value()) {
        Identifier identifier(variable_declaration, variable_declaration.identifier_name);
        translate_assignment_to_llvm(block, identifier, variable_declaration.initial_value.value());
    }
    return block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_constant_declaration_to_llvm(
    llvm::BasicBlock* block,
    const ConstDeclaration& const_declaration
) {
    llvm::Type* llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(const_declaration.typesignature);
    llvm::IRBuilder<> builder(block);
    llvm::AllocaInst* alloca_inst = builder.CreateAlloca(llvm_type, nullptr);
    scope_context.store_local_constant(const_declaration, alloca_inst);
    Identifier identifier(const_declaration, const_declaration.identifier_name);
    translate_assignment_to_llvm(block, identifier, const_declaration.value);
    return block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_assignment_to_llvm(
    llvm::BasicBlock* block,
    const Assignment& assignment
) {
    translate_assignment_to_llvm(
        block,
        assignment.assignment_target, 
        assignment.assigned_value
    );
    return block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_assignment_to_llvm(
    llvm::BasicBlock* block,
    const Expression& target, 
    const Expression& source
) {
    auto target_type_opt = program_representation.resolve_expression_type(target, scope_context.raw_scope_context);
    auto source_type_opt = program_representation.resolve_expression_type(source, scope_context.raw_scope_context);
    assert_type_deduction_success_in_backend_layer(target_type_opt.has_value());
    assert_type_deduction_success_in_backend_layer(source_type_opt.has_value());
    TypeSignature target_type = target_type_opt.value();
    TypeSignature source_type = source_type_opt.value();
    TranslatedExpression translated_target = translate_expression_to_llvm(block, target);
    TranslatedExpression translated_source = translate_expression_to_llvm(block, source);
    llvm::IRBuilder<> builder(block);
    TypeManipulationsLLVMTranslator type_manipulations_llvm_translator(program_representation, type_definitions_llvm_translator);
    TranslatedExpression casted_source = type_manipulations_llvm_translator.cast_translated_expression_to_another_type_in_llvm(
        block,
        translated_source,
        source_type,
        target_type
    );
    assert(translated_target.address != nullptr);
    builder.CreateStore(casted_source.value, translated_target.address);
    return block;
}