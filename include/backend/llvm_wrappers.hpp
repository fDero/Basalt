//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "backend/translated_expression.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>

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
    llvm::Value* proper_array_address_not_ptr,
    llvm::Value* index
) {
    std::vector<llvm::Value*> indices;
    indices.push_back(llvm::ConstantInt::get(builder.getInt32Ty(), 0));
    indices.push_back(index);
    return builder.CreateGEP(proper_array_address_not_ptr, indices);
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