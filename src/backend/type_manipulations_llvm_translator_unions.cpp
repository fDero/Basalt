//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/type_manipulations_llvm_translator.hpp"
#include "backend/llvm_wrappers.hpp"

#include <iostream>

TranslatedExpression TypeManipulationsLLVMTranslator::test_concrete_type_of_union_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    const TypeSignature& type_to_check
) {
    llvm::IRBuilder<> builder(block);
    llvm::Value* union_address = get_llvm_address(builder, union_expression);
    llvm::Value* union_header_address = builder.CreateStructGEP(union_address, 0);
    llvm::Value* union_header = builder.CreateLoad(union_header_address);
    llvm::GlobalVariable* expected_type_info = type_definitions_llvm_translator.fetch_type_info(type_to_check);
    llvm::Value* casted_expected_type_info = builder.CreateBitCast(expected_type_info, union_header->getType());
    llvm::Value* is_operator_result = builder.CreateICmpEQ(union_header, casted_expected_type_info);
    std::vector<llvm::GlobalVariable*> alternative_type_infos = type_definitions_llvm_translator
        .fetch_all_type_infos_for_non_union_compatible_types(type_to_check);
    for (llvm::GlobalVariable* alternative_type_info : alternative_type_infos) {
        llvm::Value* casted_alternative_type_info = builder.CreateBitCast(alternative_type_info, union_header->getType());
        llvm::Value* is_current_type = builder.CreateICmpEQ(union_header, casted_alternative_type_info);
        is_operator_result = builder.CreateOr(is_operator_result, is_current_type);
    }
    return is_operator_result;
}

TranslatedExpression TypeManipulationsLLVMTranslator::cast_union_expression_to_another_union_type_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    const TypeSignature& dest_type
) {
    llvm::Type* llvm_dest_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(dest_type);
    llvm::IRBuilder<> builder(block);
    llvm::Value* src_union_address = get_llvm_address(builder, union_expression);
    llvm::Value* src_union_header_address = builder.CreateStructGEP(src_union_address, 0);
    llvm::Value* src_union_payload_address = builder.CreateStructGEP(src_union_address, 0);
    llvm::Value* src_union_header = builder.CreateLoad(src_union_header_address);
    llvm::Value* src_union_payload = builder.CreateLoad(src_union_payload_address);
    llvm::Value* dest_union_address = builder.CreateAlloca(llvm_dest_type);
    llvm::Value* dest_union_header_address = builder.CreateStructGEP(dest_union_address, 0);
    llvm::Value* dest_union_payload_address = builder.CreateStructGEP(dest_union_address, 0);
    builder.CreateStore(src_union_header, dest_union_header_address);
    builder.CreateStore(src_union_payload, dest_union_payload_address);
    llvm::Value* dest_union_value = builder.CreateLoad(dest_union_address);
    return {dest_union_value, dest_union_address};
}

TranslatedExpression TypeManipulationsLLVMTranslator::cast_union_expression_to_one_of_its_alternatives_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    const TypeSignature& dest_type
) {
    llvm::Type* llvm_dest_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(dest_type);
    llvm::IRBuilder<> builder(block);
    llvm::Value* union_address = get_llvm_address(builder, union_expression);
    llvm::Value* union_payload_address = builder.CreateStructGEP(union_address, 1);
    llvm::Value* casted_union_payload_address = builder.CreateBitCast(union_payload_address, llvm_dest_type->getPointerTo());
    llvm::Value* casted_union_payload = builder.CreateLoad(casted_union_payload_address);
    return (union_expression.address == nullptr)
        ? TranslatedExpression(casted_union_payload)
        : TranslatedExpression(casted_union_payload, casted_union_payload_address);
}

TranslatedExpression TypeManipulationsLLVMTranslator::cast_simple_expression_to_union_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression simple_expression,
    const TypeSignature& soruce_type,
    const TypeSignature& dest_type
) {
    llvm::Type* llvm_dest_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(dest_type);
    llvm::IRBuilder<> builder(block);
    llvm::Value* union_address = builder.CreateAlloca(llvm_dest_type);
    llvm::Value* union_header_address = builder.CreateStructGEP(union_address, 0);
    llvm::GlobalVariable* type_info = type_definitions_llvm_translator.fetch_type_info(soruce_type);
    llvm::Type* union_header_type = union_header_address->getType()->getPointerElementType();
    llvm::Value* casted_type_info = builder.CreateBitCast(type_info, union_header_type);
    builder.CreateStore(casted_type_info, union_header_address);
    llvm::Value* union_payload_address = builder.CreateStructGEP(union_address, 1);
    llvm::Value* casted_union_payload_address = builder.CreateBitCast(union_payload_address, simple_expression.value->getType()->getPointerTo());
    builder.CreateStore(simple_expression.value, casted_union_payload_address);
    llvm::Value* union_value = builder.CreateLoad(union_address);
    return union_value;
}