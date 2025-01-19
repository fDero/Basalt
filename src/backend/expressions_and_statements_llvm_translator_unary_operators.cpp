//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_manipulations_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_not_to_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& boolean_not
) {
    TranslatedExpression operand = translate_expression_to_llvm(block, boolean_not.operand);
    llvm::Value* operand_value = operand.value;
    llvm::IRBuilder<> builder(block);
    return builder.CreateNot(operand_value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_minus_sign_to_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& minus_sign
) {
    TranslatedExpression operand = translate_expression_to_llvm(block, minus_sign.operand);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    llvm::Type* llvm_double_type = llvm::Type::getDoubleTy(context);
    llvm::Type* llvm_int_type = llvm::Type::getInt64Ty(context);
    llvm::Constant* llvm_double_0 = llvm::ConstantFP::get(llvm_double_type, 0.0);
    llvm::Constant* llvm_int_0 = llvm::ConstantInt::get(llvm_int_type, 0);
    llvm::IRBuilder<> builder(block);
    return program_representation.is_unary_operator_over_float_operand(minus_sign, raw_scope_context)
        ? builder.CreateFSub(llvm_double_0, operand.value) 
        : builder.CreateSub(llvm_int_0, operand.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_plus_sign_to_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& minus_sign
) {
    TranslatedExpression operand = translate_expression_to_llvm(block, minus_sign.operand);
    return operand.value;
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_increment_prefix_to_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& minus_sign
) {
    TranslatedExpression operand = translate_expression_to_llvm(block, minus_sign.operand);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    llvm::Type* llvm_double_type = llvm::Type::getDoubleTy(context);
    llvm::Type* llvm_int_type = llvm::Type::getInt64Ty(context);
    llvm::Constant* llvm_double_1 = llvm::ConstantFP::get(llvm_double_type, 0.0);
    llvm::Constant* llvm_int_1 = llvm::ConstantInt::get(llvm_int_type, 0);
    llvm::IRBuilder<> builder(block);
    return program_representation.is_unary_operator_over_float_operand(minus_sign, raw_scope_context)
        ? builder.CreateFAdd(operand.value, llvm_double_1) 
        : builder.CreateAdd(operand.value, llvm_int_1);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_decrement_prefix_to_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& minus_sign
) {
    TranslatedExpression operand = translate_expression_to_llvm(block, minus_sign.operand);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    llvm::Type* llvm_double_type = llvm::Type::getDoubleTy(context);
    llvm::Type* llvm_int_type = llvm::Type::getInt64Ty(context);
    llvm::Constant* llvm_double_1 = llvm::ConstantFP::get(llvm_double_type, 0.0);
    llvm::Constant* llvm_int_1 = llvm::ConstantInt::get(llvm_int_type, 0);
    llvm::IRBuilder<> builder(block);
    return program_representation.is_unary_operator_over_float_operand(minus_sign, raw_scope_context)
        ? builder.CreateFSub(operand.value, llvm_double_1) 
        : builder.CreateSub(operand.value, llvm_int_1);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_ptr_dereference_to_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& minus_sign
) {
    TranslatedExpression operand = translate_expression_to_llvm(block, minus_sign.operand);
    llvm::IRBuilder<> builder(block);
    return TranslatedExpression(
        builder.CreateLoad(operand.value), 
        operand.value
    );  
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_addressof_to_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& minus_sign
) {
    TranslatedExpression operand = translate_expression_to_llvm(block, minus_sign.operand);
    return operand.address;
}