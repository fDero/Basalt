//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <string>
#include <vector>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include "backend/translated_expression.hpp"


inline llvm::Value* get_llvm_address(
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

inline llvm::Value* create_array_gep(
    llvm::IRBuilder<>& builder,
    llvm::Type* array_type,
    llvm::Value* proper_array_address_not_ptr,
    llvm::Value* index
) {
    std::vector<llvm::Value*> indices;
    indices.push_back(llvm::ConstantInt::get(builder.getInt32Ty(), 0));
    indices.push_back(index);
    return builder.CreateGEP(array_type, proper_array_address_not_ptr, indices);
}

inline llvm::Value* create_vector_gep(
    llvm::IRBuilder<>& builder,
    llvm::Type* element_type,
    llvm::Value* ptr_to_first_elem,
    llvm::Value* index
) {
    std::vector<llvm::Value*> indices;
    indices.push_back(index);
    return builder.CreateGEP(element_type, ptr_to_first_elem, indices);
}

inline llvm::BasicBlock* createBlockAfter(
    llvm::LLVMContext& context,
    const std::string& block_name,
    llvm::BasicBlock* prevBlock
) {
    auto new_block = llvm::BasicBlock::Create(context, block_name, prevBlock->getParent());
    new_block->moveAfter(prevBlock);
    return new_block;
}
