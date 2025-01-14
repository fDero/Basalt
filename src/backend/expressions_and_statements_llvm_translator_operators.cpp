//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

using TranslatedExpression = ExpressionsAndStatementsLLVMTranslator::TranslatedExpression;


TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_binary_operator_into_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& expr
) {
    llvm::Value* left = translate_expression_into_llvm(block, expr.left_operand).value;
    llvm::Value* right = translate_expression_into_llvm(block, expr.right_operand).value;
    OperatorKind operator_kind = operator_kinds.at(expr.operator_text);
    llvm::IRBuilder<> builder(block);
    switch (operator_kind) {
        case OperatorKind::plus_operator:         return builder.CreateAdd(left, right);
        case OperatorKind::minus_operator:        return builder.CreateSub(left, right);
        case OperatorKind::mul_operator:          return builder.CreateMul(left, right);
        case OperatorKind::div_operator:          return builder.CreateSDiv(left, right);
        case OperatorKind::mod_operator:          return builder.CreateSRem(left, right);
        case OperatorKind::less_then:             return builder.CreateICmpSLT(left, right);
        case OperatorKind::greater_then:          return builder.CreateICmpSGT(left, right);
        case OperatorKind::less_then_or_equal:    return builder.CreateICmpSLE(left, right);
        case OperatorKind::greater_then_or_equal: return builder.CreateICmpSGE(left, right);
        case OperatorKind::equal:                 return builder.CreateICmpEQ(left, right);
        case OperatorKind::not_equal:             return builder.CreateICmpNE(left, right);
        case OperatorKind::and_operator:          return builder.CreateAnd(left, right);
        case OperatorKind::or_operator:           return builder.CreateOr(left, right);
        case OperatorKind::xor_operator:          return builder.CreateXor(left, right);
        case OperatorKind::pow_operator:          return translate_pow_operator_into_llvm(block, expr);
        default: throw_unrecognized_binary_operator(expr);
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_type_operator_into_llvm(
    llvm::BasicBlock* block,
    const TypeOperator& expr
) {
    switch (expr.operator_text.front()) {
        case 'i': return translate_is_operator_into_llvm(block, expr);
        case 'a': return translate_as_operator_into_llvm(block, expr);
        default: throw_unrecognized_type_operator(expr);
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_unary_operator_into_llvm(
    llvm::BasicBlock* block,
    const UnaryOperator& expr
) {
    TranslatedExpression operand = translate_expression_into_llvm(block, expr.operand);
    llvm::Value* operand_value = operand.value;
    OperatorKind operator_kind = operator_kinds.at(expr.operator_text);
    llvm::IRBuilder<> builder(block);
    switch (operator_kind) {
        case OperatorKind::boolean_not:            return builder.CreateNot(operand_value);
        case OperatorKind::math_increment:         return builder.CreateAdd(operand_value, llvm::ConstantInt::get(operand_value->getType(), 1));
        case OperatorKind::math_decrement:         return builder.CreateSub(operand_value, llvm::ConstantInt::get(operand_value->getType(), 1));
        case OperatorKind::pointer_dereference_op: return { builder.CreateLoad(operand_value), operand_value };
        case OperatorKind::address_of:             return operand.address;
        default: throw_unrecognized_unary_operator(expr);
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_is_operator_into_llvm(
    llvm::BasicBlock* block,
    const TypeOperator& expr
) {
    llvm::Function* is_operator_function = callable_codeblocks_llvm_translator
        .translate_is_builtin_operator_as_llvm_function(expr);
    llvm::Value* operand = translate_expression_into_llvm(block, expr.expression).value;
    llvm::IRBuilder<> builder(block);
    return builder.CreateCall(is_operator_function, {operand});
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_as_operator_into_llvm(
    llvm::BasicBlock* block,
    const TypeOperator& expr
) {
    llvm::Function* as_operator_function = callable_codeblocks_llvm_translator
        .translate_as_builtin_operator_as_llvm_function(expr);
    llvm::Value* operand = translate_expression_into_llvm(block, expr.expression).value;
    llvm::IRBuilder<> builder(block);
    return builder.CreateCall(as_operator_function, {operand});
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_pow_operator_into_llvm(
    llvm::BasicBlock* block,
    const BinaryOperator& expr
) {
    llvm::Function* pow_operator_function = callable_codeblocks_llvm_translator
        .translate_pow_builtin_operator_as_llvm_function(expr);
    llvm::Value* left_operand = translate_expression_into_llvm(block, expr.left_operand).value;
    llvm::Value* right_operand = translate_expression_into_llvm(block, expr.right_operand).value;
    llvm::IRBuilder<> builder(block);
    return builder.CreateCall(pow_operator_function, {left_operand, right_operand});
}