//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

static llvm::Value* get_llvm_address(
    llvm::IRBuilder<>& builder,
    const TranslatedExpression& expr
) {
    llvm::Value* address = expr.address;
    if (address == nullptr) {
        address = builder.CreateAlloca(expr.value->getType());
        builder.CreateStore(expr.value, address);
    }
    return address;
}

static llvm::Value* create_array_gep(
    llvm::IRBuilder<>& builder,
    llvm::Value* proper_array_address_not_ptr,
    llvm::Value* index
) {
    std::vector<llvm::Value*> indices;
    indices.push_back(llvm::ConstantInt::get(builder.getInt32Ty(), 0));
    indices.push_back(index);
    return builder.CreateGEP(proper_array_address_not_ptr, indices);
}

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
    llvm::IRBuilder<> builder(block);
    llvm::Value* storage_address = get_llvm_address(builder, storage);
    llvm::Value* index_value = index.value;
    std::vector<llvm::Value*> indices;
    llvm::Value* element_address = create_array_gep(builder, storage_address, index_value);
    llvm::Value* element_value = builder.CreateLoad(element_address);
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
    llvm::Value* slice_address = get_llvm_address(builder, slice);
    llvm::Value* index_value = index.value;
    llvm::Value* storage_address_address = builder.CreateStructGEP(slice_address, 1);
    llvm::Value* storage_address = builder.CreateLoad(storage_address_address);
    llvm::Value* element_address = builder.CreateGEP(storage_address, index_value);
    llvm::Value* element_value = builder.CreateLoad(element_address);
    return (slice.address == nullptr)
        ? TranslatedExpression(element_value)
        : TranslatedExpression(element_value, element_address);
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
    llvm::IRBuilder<> builder(block);
    llvm::Value* target_address = get_llvm_address(builder, target);
    llvm::Value* field_address = builder.CreateStructGEP(target_address, field_index);
    llvm::Value* field_value = builder.CreateLoad(field_address);
    return (target.address == nullptr)
        ? TranslatedExpression(field_value)
        : TranslatedExpression(field_value, field_address);
}