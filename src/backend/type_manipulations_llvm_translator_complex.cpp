//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/type_manipulations_llvm_translator.hpp"
#include "backend/llvm_wrappers.hpp"
#include "errors/internal_errors.hpp"

TranslatedExpression TypeManipulationsLLVMTranslator::cast_pointer_to_array_to_slice_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression pointer_to_array_expression,
    const TypeSignature& cast_dest,
    const PointerType& soruce_pointer_to_array_type
) {
    assert_typesignature_is<SliceType>(cast_dest);
    assert_typesignature_is<ArrayType>(soruce_pointer_to_array_type.pointed_type);
    const ArrayType& source_array_type = soruce_pointer_to_array_type.pointed_type.get<ArrayType>();
    size_t array_length = source_array_type.array_length;
    llvm::IRBuilder<> builder(block);
    llvm::Type* llvm_slice_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(cast_dest);
    llvm::Value* newly_created_slice_address = builder.CreateAlloca(llvm_slice_type);
    llvm::Value* newly_created_slice_len_address = builder.CreateStructGEP(newly_created_slice_address, 0);
    llvm::Value* newly_created_slice_storage_address_address = builder.CreateStructGEP(newly_created_slice_address, 1);
    llvm::Type* llvm_int_type = type_definitions_llvm_translator.get_int_llvm_type();
    llvm::Value* llvm_slice_size = llvm::ConstantInt::get(llvm_int_type, array_length);
    builder.CreateStore(llvm_slice_size, newly_created_slice_len_address);
    builder.CreateStore(pointer_to_array_expression.value, newly_created_slice_storage_address_address);
    return builder.CreateLoad(newly_created_slice_address);
}

TranslatedExpression TypeManipulationsLLVMTranslator::cast_slice_of_chars_to_string_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression slice_of_chars_expression
) {
    llvm::IRBuilder<> builder(block);
    llvm::Value* slice_address = get_llvm_address(builder, slice_of_chars_expression);
    llvm::Type* llvm_string_type = type_definitions_llvm_translator.get_string_llvm_type();
    llvm::Value* casted_string_address = builder.CreateBitCast(slice_address, llvm_string_type->getPointerTo());
    return builder.CreateLoad(casted_string_address);
}

TranslatedExpression TypeManipulationsLLVMTranslator::cast_string_to_raw_string_in_llvm(
    llvm::BasicBlock* block,
    TranslatedExpression string_expression
) {
    llvm::IRBuilder<> builder(block);
    llvm::Value* string_address = get_llvm_address(builder, string_expression);
    llvm::Type* llvm_raw_string_type = type_definitions_llvm_translator.get_raw_string_llvm_type();
    llvm::Value* first_char_address_address = builder.CreateStructGEP(string_address, 1);
    llvm::Value* first_char_address = builder.CreateLoad(first_char_address_address);
    llvm::Value* casted_raw_string_address = builder.CreateBitCast(first_char_address, llvm_raw_string_type);
    return builder.CreateLoad(casted_raw_string_address);
}