//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <algorithm>
#include <cstdint>
#include <vector>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Alignment.h>
#include <llvm/Support/Casting.h>

#include "backend/llvm_wrappers.hpp"
#include "backend/translated_expression.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "backend/type_manipulations_llvm_translator.hpp"
#include "language/typesignatures.hpp"


TranslatedExpression TypeManipulationsLLVMTranslator::test_concrete_type_of_union_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    const TypeSignature& type_to_check
) {
    llvm::IRBuilder<> builder(block);
    llvm::StructType* union_type = llvm::cast<llvm::StructType>(union_expression.value->getType());
    llvm::Value* union_address = get_llvm_address(builder, union_expression);
    llvm::Value* union_header_address = builder.CreateStructGEP(union_type, union_address, 0);
    llvm::Value* union_header = builder.CreateLoad(union_type->getElementType(0), union_header_address);
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
    llvm::StructType* src_union_type = llvm::cast<llvm::StructType>(union_expression.value->getType());
    llvm::StructType* dest_union_type = llvm::cast<llvm::StructType>(
        type_definitions_llvm_translator.translate_typesignature_to_llvm_type(dest_type)
    );
    llvm::IRBuilder<> builder(block);
    llvm::Value* src_union_address = get_llvm_address(builder, union_expression);
    llvm::Value* src_union_header_address = builder.CreateStructGEP(src_union_type, src_union_address, 0);
    llvm::Value* src_union_payload_address = builder.CreateStructGEP(src_union_type, src_union_address, 1);
    llvm::Value* src_union_header = builder.CreateLoad(src_union_type->getElementType(0), src_union_header_address);
    llvm::Value* dest_union_address = builder.CreateAlloca(dest_union_type);
    llvm::Value* dest_union_header_address = builder.CreateStructGEP(dest_union_type, dest_union_address, 0);
    llvm::Value* dest_union_payload_address = builder.CreateStructGEP(dest_union_type, dest_union_address, 1);
    builder.CreateStore(src_union_header, dest_union_header_address);
    uint64_t src_payload_size = llvm::cast<llvm::ArrayType>(src_union_type->getElementType(1))->getNumElements();
    uint64_t dest_payload_size = llvm::cast<llvm::ArrayType>(dest_union_type->getElementType(1))->getNumElements();
    builder.CreateMemCpy(
        dest_union_payload_address, llvm::MaybeAlign(),
        src_union_payload_address, llvm::MaybeAlign(),
        std::min(src_payload_size, dest_payload_size)
    );
    llvm::Value* dest_union_value = builder.CreateLoad(dest_union_type, dest_union_address);
    return {dest_union_value, dest_union_address};
}

TranslatedExpression TypeManipulationsLLVMTranslator::cast_union_expression_as_ref_to_another_union_type_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    const TypeSignature& dest_type
) {
    llvm::IRBuilder<> builder(block);
    llvm::Value* union_address = get_llvm_address(builder, union_expression);
    llvm::Type* llvm_dest_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(dest_type);
    llvm::Value* casted_union_payload_address = builder.CreateBitCast(union_address, llvm_dest_type->getPointerTo());
    llvm::Value* casted_union_payload = builder.CreateLoad(llvm_dest_type, casted_union_payload_address);
    return (union_expression.address == nullptr)
        ? TranslatedExpression(casted_union_payload)
        : TranslatedExpression(casted_union_payload, casted_union_payload_address);
}

TranslatedExpression TypeManipulationsLLVMTranslator::cast_union_expression_to_one_of_its_alternatives_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    const TypeSignature& dest_type
) {
    llvm::Type* llvm_dest_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(dest_type);
    llvm::StructType* union_type = llvm::cast<llvm::StructType>(union_expression.value->getType());
    llvm::IRBuilder<> builder(block);
    llvm::Value* union_address = get_llvm_address(builder, union_expression);
    llvm::Value* union_payload_address = builder.CreateStructGEP(union_type, union_address, 1);
    llvm::Value* casted_union_payload_address = builder.CreateBitCast(union_payload_address, llvm_dest_type->getPointerTo());
    llvm::Value* casted_union_payload = builder.CreateLoad(llvm_dest_type, casted_union_payload_address);
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
    llvm::StructType* union_type = llvm::cast<llvm::StructType>(
        type_definitions_llvm_translator.translate_typesignature_to_llvm_type(dest_type)
    );
    llvm::IRBuilder<> builder(block);
    llvm::Value* union_address = builder.CreateAlloca(union_type);
    llvm::Value* union_header_address = builder.CreateStructGEP(union_type, union_address, 0);
    llvm::GlobalVariable* type_info = type_definitions_llvm_translator.fetch_type_info(soruce_type);
    llvm::Value* casted_type_info = builder.CreateBitCast(type_info, union_type->getElementType(0));
    builder.CreateStore(casted_type_info, union_header_address);
    llvm::Value* union_payload_address = builder.CreateStructGEP(union_type, union_address, 1);
    llvm::Value* casted_union_payload_address = builder.CreateBitCast(union_payload_address, simple_expression.value->getType()->getPointerTo());
    builder.CreateStore(simple_expression.value, casted_union_payload_address);
    llvm::Value* union_value = builder.CreateLoad(union_type, union_address);
    return union_value;
}
