//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_literal_into_llvm(
    llvm::BasicBlock* block,
    const BoolLiteral& expr
) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), expr.value);   
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_character_literal_into_llvm(
    llvm::BasicBlock* block,
    const CharLiteral& expr
) {
    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), expr.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_floating_literal_into_llvm(
    llvm::BasicBlock* block, 
    const FloatLiteral& expr
) {
    return llvm::ConstantFP::get(context, llvm::APFloat(expr.value));
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_integer_literal_into_llvm(
    llvm::BasicBlock* block, 
    const IntLiteral& expr
) {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), expr.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_string_literal_into_llvm(
    llvm::BasicBlock* block,
    const StringLiteral& expr
) {
    return llvm::ConstantDataArray::getString(context, expr.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_identifier_into_llvm(
    llvm::BasicBlock* block,
    const Identifier& identifier
) {
    llvm::AllocaInst* address = scope_context.resolve_object_allocation(identifier.name);
    llvm::IRBuilder<> builder(block);
    llvm::Value* value = builder.CreateLoad(address);
    return {value, address};
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_array_literal_into_llvm(
    llvm::BasicBlock* block,
    const ArrayLiteral& expr
) {
    llvm::Type* elementType = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(expr.stored_type);
    llvm::ArrayType* arrayType = llvm::ArrayType::get(elementType, expr.elements.size());
    llvm::IRBuilder<> builder(block);
    llvm::Value* llvm_array_reference = builder.CreateAlloca(arrayType);
    std::vector<llvm::Value*> array_elements;
    for (size_t index = 0; index < expr.elements.size(); index++) {
        llvm::Value* element_index_as_expr = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), index);
        const Expression& element = expr.elements[index];
        llvm::Value* array_element = translate_expression_into_llvm(block, element).value;
        llvm::Value* ptr_to_array_cell = builder.CreateGEP(arrayType, llvm_array_reference, element_index_as_expr);
        builder.CreateStore(array_element, ptr_to_array_cell);
    }
    return llvm_array_reference;
}