//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/llvm_wrappers.hpp"
#include "errors/internal_errors.hpp"
#include "syntax/primitive_types.hpp"

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
        case TypeSignatureBody::Kind::primitive_type: {
            std::string primitive_type_name = storage_type.get<PrimitiveType>().type_name;
            if (primitive_type_name == string_type) return translate_square_bracket_access_from_string_to_llvm(block, expr);
            if (primitive_type_name == raw_string_type) return translate_square_bracket_access_from_raw_string_to_llvm(block, expr);
        }
        default: assert_unreachable();
    }
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_from_string_to_llvm(
    llvm::BasicBlock* block,
    const SquareBracketsAccess& expr
) {
    return translate_square_bracket_access_from_slice_to_llvm(block, expr);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_from_raw_string_to_llvm(
    llvm::BasicBlock* block,
    const SquareBracketsAccess& expr
) {
    TranslatedExpression ptr_to_first_char = translate_expression_to_llvm(block, expr.storage);
    TranslatedExpression index = translate_expression_to_llvm(block, expr.index);
    llvm::IRBuilder<> builder(block);
    llvm::Value* index_value = index.value;
    std::vector<llvm::Value*> indices;
    llvm::Value* element_address = create_vector_gep(builder, ptr_to_first_char.value, index_value);
    llvm::Value* element_value = builder.CreateLoad(element_address);
    return TranslatedExpression(element_value, element_address);
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