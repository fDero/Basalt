//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_boolean_literal_into_llvm(const BoolLiteral& expr) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), expr.value);
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_character_literal_into_llvm(const CharLiteral& expr) {
    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), expr.value);
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_floating_literal_into_llvm(const FloatLiteral& expr) {
    return llvm::ConstantFP::get(context, llvm::APFloat(expr.value));
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_identifier_into_llvm(const Identifier& expr) {
    std::string unique_id = scope_context.resolve_object_unique_id(expr.name);
    throw std::runtime_error("not yet fully implemented");
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_integer_literal_into_llvm(const IntLiteral& expr) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), expr.value);
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_string_literal_into_llvm(const StringLiteral& expr) {
    return llvm::ConstantDataArray::getString(context, expr.value);
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_array_literal_into_llvm(const ArrayLiteral& expr) {
    llvm::Type* elementType = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(expr.stored_type);
    llvm::ArrayType* arrayType = llvm::ArrayType::get(elementType, expr.elements.size());
    llvm::Value* llvm_array_reference = builder.CreateAlloca(arrayType);
    std::vector<llvm::Value*> array_elements;
    for (size_t index = 0; index < expr.elements.size(); index++) {
        llvm::Value* element_index_as_expr = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), index);
        const Expression& element = expr.elements[index];
        llvm::Value* array_element = translate_expression_into_llvm(element);
        llvm::Value* ptr_to_array_cell = builder.CreateGEP(arrayType, llvm_array_reference, element_index_as_expr);
        builder.CreateStore(array_element, ptr_to_array_cell);
    }
    return llvm_array_reference;
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_binary_operator_into_llvm(const BinaryOperator& expr) {
    llvm::Value* left = translate_expression_into_llvm(expr.left_operand);
    llvm::Value* right = translate_expression_into_llvm(expr.right_operand);
    OperatorKind operator_kind = operator_kinds.at(expr.operator_text);
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
        case OperatorKind::pow_operator:          return translate_pow_operator_into_llvm(expr);
        default: throw_unrecognized_binary_operator(expr);
    }
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_type_operator_into_llvm(const TypeOperator& expr) {
    switch (expr.operator_text.front()) {
        case 'i': return translate_is_operator_into_llvm(expr);
        case 'a': return translate_as_operator_into_llvm(expr);
        default: throw_unrecognized_type_operator(expr);
    }
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_unary_operator_into_llvm(const UnaryOperator& expr) {
    llvm::Value* operand = translate_expression_into_llvm(expr.operand);
    OperatorKind operator_kind = operator_kinds.at(expr.operator_text);
    switch (operator_kind) {
        case OperatorKind::boolean_not:            return builder.CreateNot(operand);
        case OperatorKind::math_increment:         return builder.CreateAdd(operand, llvm::ConstantInt::get(operand->getType(), 1));
        case OperatorKind::math_decrement:         return builder.CreateSub(operand, llvm::ConstantInt::get(operand->getType(), 1));
        case OperatorKind::pointer_dereference_op: return builder.CreateLoad(operand);
        case OperatorKind::address_of:             return operand;
        default: throw_unrecognized_unary_operator(expr);
    }
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_is_operator_into_llvm(const TypeOperator& expr) {
    llvm::Function* is_operator_function = callable_codeblocks_llvm_translator
        .translate_is_builtin_operator_as_llvm_function(expr);
    llvm::Value* operand = translate_expression_into_llvm(expr.expression);
    return builder.CreateCall(is_operator_function, {operand});
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_as_operator_into_llvm(const TypeOperator& expr) {
    llvm::Function* as_operator_function = callable_codeblocks_llvm_translator
        .translate_as_builtin_operator_as_llvm_function(expr);
    llvm::Value* operand = translate_expression_into_llvm(expr.expression);
    return builder.CreateCall(as_operator_function, {operand});
}

llvm::Value* ExpressionsAndStatementsLLVMTranslator::translate_pow_operator_into_llvm(const BinaryOperator& expr) {
    llvm::Function* pow_operator_function = callable_codeblocks_llvm_translator
        .translate_pow_builtin_operator_as_llvm_function(expr);
    llvm::Value* left_operand = translate_expression_into_llvm(expr.left_operand);
    llvm::Value* right_operand = translate_expression_into_llvm(expr.right_operand);
    return builder.CreateCall(pow_operator_function, {left_operand, right_operand});
}