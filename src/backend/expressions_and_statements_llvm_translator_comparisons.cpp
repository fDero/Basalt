//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

using TranslatedExpression = ExpressionsAndStatementsLLVMTranslator::TranslatedExpression;

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_LT_then_comparison(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFCmpULT(left.value, right.value)
        : builder.CreateICmpSLT(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_GT_comparison(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFCmpUGT(left.value, right.value)
        : builder.CreateICmpSGT(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_LE_comparison(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFCmpULE(left.value, right.value)
        : builder.CreateICmpSLE(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_GE_comparison(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFCmpUGE(left.value, right.value)
        : builder.CreateICmpSGE(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_EQ_comparison(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) {
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFCmpUEQ(left.value, right.value)
        : builder.CreateICmpEQ(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_NE_comparison(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) {
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context))  
        ? builder.CreateFCmpUNE(left.value, right.value)
        : builder.CreateICmpNE(left.value, right.value);
}