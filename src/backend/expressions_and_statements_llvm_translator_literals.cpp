//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "frontend/syntax.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_manipulations_llvm_translator.hpp"
#include "backend/llvm_wrappers.hpp"
#include "errors/internal_errors.hpp"

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_boolean_literal_to_llvm(
    llvm::BasicBlock* block,
    const BoolLiteral& expr
) {
    return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), expr.value);   
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_character_literal_to_llvm(
    llvm::BasicBlock* block,
    const CharLiteral& expr
) {
    return llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), expr.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_floating_literal_to_llvm(
    llvm::BasicBlock* block, 
    const FloatLiteral& expr
) {
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), expr.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_integer_literal_to_llvm(
    llvm::BasicBlock* block, 
    const IntLiteral& expr
) {
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), expr.value);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_string_literal_to_llvm(
    llvm::BasicBlock* block,
    const StringLiteral& expr
) {
    auto array_of_char_val = llvm::ConstantDataArray::getString(context, expr.value);
    auto global_string_var = new llvm::GlobalVariable(
        llvm_module,
        array_of_char_val->getType(),
        true,
        llvm::GlobalValue::PrivateLinkage,
        array_of_char_val,
        expr.value
    );
    llvm::Type* char_ptr_type = type_definitions_llvm_translator.get_char_llvm_type()->getPointerTo();
    llvm::Type* string_type = type_definitions_llvm_translator.get_string_llvm_type();
    auto ptr_to_first_char = llvm::ConstantExpr::getBitCast(global_string_var, char_ptr_type);
    llvm::IRBuilder<> builder(block);
    llvm::Value* string_address = builder.CreateAlloca(string_type);
    llvm::Value* string_len_address = builder.CreateStructGEP(string_type, string_address, 0);
    llvm::Value* string_data_address = builder.CreateStructGEP(string_type, string_address, 1);
    llvm::Value* string_len = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), expr.value.size());
    builder.CreateStore(string_len, string_len_address);
    builder.CreateStore(ptr_to_first_char, string_data_address);
    llvm::Value* string_value = builder.CreateLoad(string_address);
    return {string_value, string_address};
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_identifier_to_llvm(
    llvm::BasicBlock* block,
    const Identifier& identifier
) {
    llvm::AllocaInst* address = scope_context.resolve_object_allocation(identifier.name);
    llvm::IRBuilder<> builder(block);
    llvm::Value* value = builder.CreateLoad(address);
    return {value, address};
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_array_literal_to_llvm(
    llvm::BasicBlock* block,
    const ArrayLiteral& expr
) {
    const TypeSignature& stored_type = expr.stored_type;
    llvm::Type* llvm_stored_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(stored_type);
    llvm::ArrayType* llvm_array_type = llvm::ArrayType::get(llvm_stored_type, expr.elements.size());
    llvm::IRBuilder<> builder(block);
    llvm::Value* llvm_array_address = builder.CreateAlloca(llvm_array_type);
    TypeManipulationsLLVMTranslator type_manipulations_llvm_translator(program_representation, type_definitions_llvm_translator);
    for (size_t index = 0; index < expr.elements.size(); index++) {
        llvm::Value* element_index_as_expr = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), index);
        const Expression& element = expr.elements[index];
        auto element_type_opt = program_representation.resolve_expression_type(element, scope_context.raw_scope_context);
        assert_type_deduction_success_in_backend_layer(element_type_opt.has_value());
        const TypeSignature& element_type = element_type_opt.value();
        TranslatedExpression array_element = translate_expression_to_llvm(block, element);
        TranslatedExpression casted_array_element = type_manipulations_llvm_translator
            .cast_translated_expression_to_another_type_in_llvm(block, array_element, element_type, stored_type);
        llvm::Value* ptr_to_array_cell = create_array_gep(builder, llvm_array_address, element_index_as_expr);
        builder.CreateStore(casted_array_element.value, ptr_to_array_cell);
    }
    llvm::Value* llvm_array_value = builder.CreateLoad(llvm_array_address);
    return {llvm_array_value, llvm_array_address};
}