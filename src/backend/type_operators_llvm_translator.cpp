//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/type_operators_llvm_translator.hpp"

TypeOperatorsLLVMTranslator::TypeOperatorsLLVMTranslator(
    ProgramRepresentation& program_representation, 
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator
)
    : program_representation(program_representation)
    , type_definitions_llvm_translator(type_definitions_llvm_translator)
{ }

[[nodiscard]] TranslatedExpression TypeOperatorsLLVMTranslator::translate_is_operator_to_llvm_value(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    const TypeSignature& type_to_check
) {
    llvm::IRBuilder<> builder(block);
    llvm::Value* union_address = union_expression.address;
    if (union_address == nullptr) {
        union_address = builder.CreateAlloca(union_expression.value->getType());
        builder.CreateStore(union_expression.value, union_address);
    }
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

[[nodiscard]] TranslatedExpression TypeOperatorsLLVMTranslator::translate_as_operator_to_llvm_value(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    llvm::Type* llvm_type_to_cast_to,
    const TypeSignature& type_to_cast_to
) {
    llvm::IRBuilder<> builder(block);
    if (program_representation.is_union(type_to_cast_to)) {
        return builder.CreateBitCast(union_expression.value, llvm_type_to_cast_to);
    }
    llvm::Value* union_address = union_expression.address;
    if (union_address == nullptr) {
        union_address = builder.CreateAlloca(union_expression.value->getType());
        builder.CreateStore(union_expression.value, union_address);
    }
    llvm::Value* union_payload_address = builder.CreateStructGEP(union_address, 1);
    llvm::Value* union_payload_casted_address = builder.CreateBitCast(union_payload_address, llvm_type_to_cast_to->getPointerTo());
    llvm::Value* union_payload_casted_value = builder.CreateLoad(union_payload_casted_address);
    return TranslatedExpression(union_payload_casted_value, union_payload_casted_address);
}