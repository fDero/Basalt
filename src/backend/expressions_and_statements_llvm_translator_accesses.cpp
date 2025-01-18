//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_to_llvm(
    llvm::BasicBlock* block,
    const SquareBracketsAccess& expr
) {
    std::optional<TypeSignature> storage_type_opt = program_representation
        .resolve_expression_type(expr.storage, scope_context.raw_scope_context);
    assert_type_deduction_success_in_backend_layer(storage_type_opt.has_value());
    TypeSignature storage_type = storage_type_opt.value();
    switch (storage_type.typesiganture_kind()) {
        case TypeSignatureBody::Kind::array_type: return translate_square_bracket_access_from_array_to_llvm(block, expr);
        case TypeSignatureBody::Kind::slice_type: return translate_square_bracket_access_from_slice_to_llvm(block, expr);
        default: assert_unreachable();
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_from_array_to_llvm(
    llvm::BasicBlock* block,
    const SquareBracketsAccess& expr
) {
    TranslatedExpression storage = translate_expression_to_llvm(block, expr.storage);
    TranslatedExpression index = translate_expression_to_llvm(block, expr.index);
    llvm::Value* storage_address = storage.address;
    llvm::IRBuilder<> builder(block);
    if (storage_address == nullptr) {
        storage_address = builder.CreateLoad(storage.value, nullptr);
    }
    llvm::Value* index_value = index.value;
    llvm::Value* element_address = builder.CreateGEP(storage_address, index_value);
    llvm::Value* element_value = builder.CreateLoad(element_address, nullptr);
    return (storage.address == nullptr)
        ? TranslatedExpression(element_value)
        : TranslatedExpression(element_value, element_address);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_from_slice_to_llvm(
    llvm::BasicBlock* block,
    const SquareBracketsAccess& expr
) {
    TranslatedExpression slice = translate_expression_to_llvm(block, expr.storage);
    TranslatedExpression index = translate_expression_to_llvm(block, expr.index);
    llvm::IRBuilder<> builder(block);
    llvm::Value* storage_address = slice.value;
    llvm::Value* index_value = index.value;
    llvm::Value* element_address = builder.CreateGEP(storage_address, index_value);
    llvm::Value* element_value = builder.CreateLoad(element_address, nullptr);
    return { element_value, element_address };
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_dot_member_access_to_llvm(
    llvm::BasicBlock* block,
    const DotMemberAccess& expr
) {
    DotMemberAccess normalized_dot_member_access = program_representation
        .normalize_dot_member_access(expr, scope_context.raw_scope_context);
    size_t field_index = program_representation
        .resolve_field_index(normalized_dot_member_access, scope_context.raw_scope_context);
    TranslatedExpression target = translate_expression_to_llvm(block, normalized_dot_member_access.struct_value);
    llvm::Value* target_address = target.address;
    llvm::IRBuilder<> builder(block);
    if (target_address == nullptr) {
        target_address = builder.CreateAlloca(target.value->getType());
        builder.CreateStore(target.value, target_address);
    }
    llvm::Value* field_address = builder.CreateStructGEP(target_address, field_index);
    llvm::Value* field_value = builder.CreateLoad(field_address);
    return (target.address == nullptr)
        ? TranslatedExpression(field_value)
        : TranslatedExpression(field_value, field_address);
}