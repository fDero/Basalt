//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/codeblock_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

llvm::Value* CodeBlockLLVMTranslator::translate_boolean_literal_into_llvm(const BoolLiteral& expr) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), expr.value);
}

llvm::Value* CodeBlockLLVMTranslator::translate_character_literal_into_llvm(const CharLiteral& expr) {
    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), expr.value);
}

llvm::Value* CodeBlockLLVMTranslator::translate_floating_literal_into_llvm(const FloatLiteral& expr) {
    return llvm::ConstantFP::get(context, llvm::APFloat(expr.value));
}

llvm::Value* CodeBlockLLVMTranslator::translate_identifier_into_llvm(const Identifier& expr) {
    std::string unique_id = scope_context.resolve_object_unique_id(expr.name);
}

llvm::Value* CodeBlockLLVMTranslator::translate_integer_literal_into_llvm(const IntLiteral& expr) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), expr.value);
}

llvm::Value* CodeBlockLLVMTranslator::translate_string_literal_into_llvm(const StringLiteral& expr) {
    return llvm::ConstantDataArray::getString(context, expr.value);
}

llvm::Value* CodeBlockLLVMTranslator::translate_array_literal_into_llvm(const ArrayLiteral& expr) {
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

llvm::Value* CodeBlockLLVMTranslator::translate_expr_function_call_into_llvm(const FunctionCall& expr) {
    
}

llvm::Value* CodeBlockLLVMTranslator::translate_binary_operator_into_llvm(const BinaryOperator& expr) {
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
        //case OperatorKind::pow_operator:
    }
}

llvm::Value* CodeBlockLLVMTranslator::translate_type_operator_into_llvm(const TypeOperator& expr) {

}

llvm::Value* CodeBlockLLVMTranslator::translate_unary_operator_into_llvm(const UnaryOperator& expr) {

}