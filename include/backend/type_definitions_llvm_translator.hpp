//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <functional>

#include "core/type_definitions_register.hpp"
#include "language/definitions.hpp"

class TypeDefinitionsLLVMTranslator {
    public:
        TypeDefinitionsLLVMTranslator(
            TypeDefinitionsRegister& type_definitions_register, 
            llvm::LLVMContext& context,
            llvm::DataLayout& data_layout
        );

        llvm::Type* translate_typesignature_to_llvm_type(const TypeSignature& type_signature);
        llvm::Type* translate_inline_union_to_llvm_type(const InlineUnion& inline_union);
        llvm::Type* translate_custom_type_to_llvm_type(const CustomType& custom_type);

        size_t compute_header_unaware_typesignature_memory_footprint(const TypeSignature& typesignature);
        
    private:
        TypeDefinitionsRegister& type_definitions_register;
        llvm::LLVMContext& context;
        llvm::DataLayout& data_layout;
        std::unordered_map<std::string, llvm::Type*> llvm_type_definitions;
};