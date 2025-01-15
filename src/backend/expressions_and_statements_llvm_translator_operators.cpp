//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

using TranslatedExpression = ExpressionsAndStatementsLLVMTranslator::TranslatedExpression;

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_is_operator_into_llvm(
    llvm::BasicBlock* block,
    const TypeOperator& expr
) {
    throw std::runtime_error("Not implemented yet");
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_as_operator_into_llvm(
    llvm::BasicBlock* block,
    const TypeOperator& as_operator
) {
    if (program_representation.is_union(as_operator.typesignature)) {
        return translate_expression_into_llvm(block, as_operator.expression);
    }
    llvm::IRBuilder<> builder(block);
    TranslatedExpression union_expression = translate_expression_into_llvm(block, as_operator.expression);
    llvm::Value* union_payload_address = builder.CreateGEP(union_expression.address, {0, 1});
    llvm::Value* union_payload_value = builder.CreateLoad(union_payload_address);
    return TranslatedExpression(union_payload_value, union_expression.address);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_pow_operator_into_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& expr
) {
    throw std::runtime_error("Not implemented yet");
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_plus_binary_operator(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFAdd(left.value, right.value)
        : builder.CreateAdd(left.value, right.value);
} 

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_minus_binary_operator(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFSub(left.value, right.value)
        : builder.CreateSub(left.value, right.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_mul_binary_operator(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFMul(left.value, right.value)
        : builder.CreateMul(left.value, right.value);
}  

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_div_binary_operator(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFDiv(left.value, right.value)
        : builder.CreateSDiv(left.value, right.value);
}  

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_mod_binary_operator(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFRem(left.value, right.value)
        : builder.CreateSRem(left.value, right.value);
}     

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_and(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    return builder.CreateAnd(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_or(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    return builder.CreateOr(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_xor(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_into_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_into_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    return builder.CreateXor(left.value, right.value);
}