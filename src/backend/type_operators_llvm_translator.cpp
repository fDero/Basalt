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
    llvm::Value* union_type_info_address = builder.CreateGEP(union_expression.address, {0, 0});
    llvm::Value* union_type_info_value = builder.CreateLoad(union_type_info_address);
    llvm::GlobalVariable* expected_type_info = type_definitions_llvm_translator.fetch_type_info(type_to_check);
    llvm::Value* is_operator_result = builder.CreateICmpEQ(union_type_info_value, expected_type_info);
    std::vector<llvm::GlobalVariable*> alternative_type_infos = type_definitions_llvm_translator
        .fetch_all_compatible_type_infos(type_to_check);
    for (llvm::GlobalVariable* alternative_type_info : alternative_type_infos) {
        llvm::Value* is_current_type = builder.CreateICmpEQ(union_type_info_value, alternative_type_info);
        is_operator_result = builder.CreateOr(is_operator_result, is_current_type);
    }
    return is_operator_result;
}

[[nodiscard]] TranslatedExpression TypeOperatorsLLVMTranslator::translate_as_operator_to_llvm_value(
    llvm::BasicBlock* block,
    TranslatedExpression union_expression,
    const TypeSignature& type_to_cast_to
) {
    if (program_representation.is_union(type_to_cast_to)) {
        return union_expression;
    }
    llvm::IRBuilder<> builder(block);
    llvm::Value* union_payload_address = builder.CreateGEP(union_expression.address, {0, 1});
    llvm::Value* union_payload_value = builder.CreateLoad(union_payload_address);
    return TranslatedExpression(union_payload_value, union_expression.address);
}