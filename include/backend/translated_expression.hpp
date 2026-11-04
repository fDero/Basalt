//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <llvm/IR/Value.h>


struct TranslatedExpression {
    TranslatedExpression(llvm::Value* value, llvm::Value* address)
        : value(value)
        , address(address)
    {}

    TranslatedExpression(llvm::Value* value)
        : TranslatedExpression(value, nullptr)
    {}

    llvm::Value* value;
    llvm::Value* address;
};
