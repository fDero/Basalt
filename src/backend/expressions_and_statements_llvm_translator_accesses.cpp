//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

using TranslatedExpression = ExpressionsAndStatementsLLVMTranslator::TranslatedExpression;

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_into_llvm(const SquareBracketsAccess& expr) {
    std::optional<TypeSignature> storage_type_opt = program_representation
        .resolve_expression_type(expr.storage, scope_context.raw_scope_context);
    assert_type_deduction_success_in_backend_layer(storage_type_opt.has_value());
    TypeSignature storage_type = storage_type_opt.value();
    switch (storage_type.typesiganture_kind()) {
        case TypeSignatureBody::Kind::array_type: return translate_square_bracket_access_from_array_into_llvm(expr);
        case TypeSignatureBody::Kind::slice_type: return translate_square_bracket_access_from_slice_into_llvm(expr);
        default: assert_unreachable();
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_from_array_into_llvm(const SquareBracketsAccess& expr) {
    TranslatedExpression storage = translate_expression_into_llvm(expr.storage);
    TranslatedExpression index = translate_expression_into_llvm(expr.index);
    llvm::Value* storage_address = storage.address;
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

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_from_slice_into_llvm(const SquareBracketsAccess& expr) {
    TranslatedExpression slice = translate_expression_into_llvm(expr.storage);
    TranslatedExpression index = translate_expression_into_llvm(expr.index);
    llvm::Value* storage_address = slice.value;
    llvm::Value* index_value = index.value;
    llvm::Value* element_address = builder.CreateGEP(storage_address, index_value);
    llvm::Value* element_value = builder.CreateLoad(element_address, nullptr);
    return { element_value, element_address };
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_dot_member_access_into_llvm(const DotMemberAccess& expr) {
    DotMemberAccess normalized_dot_member_access = program_representation
        .normalize_dot_member_access(expr, scope_context.raw_scope_context);
    size_t field_index = program_representation
        .resolve_field_index(normalized_dot_member_access, scope_context.raw_scope_context);
    TranslatedExpression target = translate_expression_into_llvm(normalized_dot_member_access.struct_value);
    llvm::Value* target_address = target.address;
    if (target_address == nullptr) {
        target_address = builder.CreateLoad(target.value, nullptr);
    }
    llvm::Value* field_address = builder.CreateGEP(target_address, {0, field_index});
    llvm::Value* field_value = builder.CreateLoad(field_address, nullptr);
    return (target.address == nullptr)
        ? TranslatedExpression(field_value)
        : TranslatedExpression(field_value, field_address);
}