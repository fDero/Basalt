//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include <optional>
#include <string>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/llvm_wrappers.hpp"
#include "backend/translated_expression.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "core/program_representation.hpp"
#include "errors/internal_errors.hpp"
#include "language/expressions.hpp"
#include "language/typesignatures.hpp"
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
    llvm::Type* char_type = type_definitions_llvm_translator.get_char_llvm_type();
    llvm::Value* element_address = create_vector_gep(builder, char_type, ptr_to_first_char.value, index_value);
    llvm::Value* element_value = builder.CreateLoad(char_type, element_address);
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
    llvm::ArrayType* array_type = llvm::cast<llvm::ArrayType>(storage.value->getType());
    llvm::Value* element_address = create_array_gep(builder, array_type, storage_address, index_value);
    llvm::Value* element_value = builder.CreateLoad(array_type->getElementType(), element_address);
    return (storage.address == nullptr)
        ? TranslatedExpression(element_value)
        : TranslatedExpression(element_value, element_address);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_square_bracket_access_from_slice_to_llvm(
    llvm::BasicBlock* block,
    const SquareBracketsAccess& expr
) {
    std::optional<TypeSignature> storage_type_opt = program_representation
        .resolve_expression_type(expr.storage, scope_context.raw_scope_context);
    assert_type_deduction_success_in_backend_layer(storage_type_opt.has_value());
    llvm::Type* element_type = (storage_type_opt->is<SliceType>())
        ? type_definitions_llvm_translator.translate_typesignature_to_llvm_type(storage_type_opt->get<SliceType>().stored_type)
        : type_definitions_llvm_translator.get_char_llvm_type();
    TranslatedExpression slice = translate_expression_to_llvm(block, expr.storage);
    TranslatedExpression index = translate_expression_to_llvm(block, expr.index);
    llvm::IRBuilder<> builder(block);
    llvm::StructType* slice_type = llvm::cast<llvm::StructType>(slice.value->getType());
    llvm::Value* slice_address = get_llvm_address(builder, slice);
    llvm::Value* index_value = index.value;
    llvm::Value* storage_address_address = builder.CreateStructGEP(slice_type, slice_address, 1);
    llvm::Value* storage_address = builder.CreateLoad(slice_type->getElementType(1), storage_address_address);
    llvm::Value* element_address = create_vector_gep(builder, element_type, storage_address, index_value);
    llvm::Value* element_value = builder.CreateLoad(element_type, element_address);
    return (slice.address == nullptr)
        ? TranslatedExpression(element_value)
        : TranslatedExpression(element_value, element_address);
}