//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_operators_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_is_operator_to_llvm(
    llvm::BasicBlock* block,
    const TypeOperator& is_operator
) {
    TranslatedExpression union_expression = translate_expression_to_llvm(block, is_operator.expression);
    TypeOperatorsLLVMTranslator type_operators_llvm_translator(program_representation, type_definitions_llvm_translator);
    return type_operators_llvm_translator.translate_is_operator_to_llvm_value(block, union_expression, is_operator.typesignature);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_as_operator_to_llvm(
    llvm::BasicBlock* block,
    const TypeOperator& as_operator
) {
    TranslatedExpression union_expression = translate_expression_to_llvm(block, as_operator.expression);
    TypeOperatorsLLVMTranslator type_operators_llvm_translator(program_representation, type_definitions_llvm_translator);
    llvm::Type* llvm_type_to_cast_to = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(as_operator.typesignature);
    return type_operators_llvm_translator.translate_as_operator_to_llvm_value(block, union_expression, llvm_type_to_cast_to, as_operator.typesignature);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_plus_binary_operator_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_to_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_to_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFAdd(left.value, right.value)
        : builder.CreateAdd(left.value, right.value);
} 

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_minus_binary_operator_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_to_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_to_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFSub(left.value, right.value)
        : builder.CreateSub(left.value, right.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_mul_binary_operator_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_to_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_to_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFMul(left.value, right.value)
        : builder.CreateMul(left.value, right.value);
}  

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_div_binary_operator_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_to_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_to_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFDiv(left.value, right.value)
        : builder.CreateSDiv(left.value, right.value);
}  

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_mod_binary_operator_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_to_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_to_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    ScopeContext& raw_scope_context = scope_context.raw_scope_context;
    return (program_representation.is_binary_operator_over_float_operands(binary_op, raw_scope_context)) 
        ? builder.CreateFRem(left.value, right.value)
        : builder.CreateSRem(left.value, right.value);
}     

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_and_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_to_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_to_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    return builder.CreateAnd(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_or_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_to_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_to_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    return builder.CreateOr(left.value, right.value);
}   

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_xor_to_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& binary_op
) { 
    TranslatedExpression left = translate_expression_to_llvm(block, binary_op.left_operand);
    TranslatedExpression right = translate_expression_to_llvm(block, binary_op.right_operand);
    llvm::IRBuilder<> builder(block);
    return builder.CreateXor(left.value, right.value);
}