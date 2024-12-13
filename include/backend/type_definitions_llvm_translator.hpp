//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <functional>

#include "core/program_representation.hpp"
#include "language/definitions.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class TypeDefinitionsLLVMTranslator {
    public:
        TypeDefinitionsLLVMTranslator(
            ProgramRepresentation& program_representation, 
            llvm::LLVMContext& context,
            llvm::Module& llvm_module
        );

        [[nodiscard]] llvm::Type* translate_typesignature_to_llvm_type(const TypeSignature& type_signature);
        [[nodiscard]] llvm::Type* translate_inline_union_to_llvm_type(const InlineUnion& inline_union);
        [[nodiscard]] llvm::Type* translate_custom_type_to_llvm_type(const CustomType& custom_type);
        [[nodiscard]] llvm::Type* translate_named_union_to_llvm_type(const UnionDefinition& union_definition);
        [[nodiscard]] llvm::Type* translate_struct_to_llvm_type(const StructDefinition& struct_definition);

        [[nodiscard]] size_t compute_header_unaware_typesignature_memory_footprint(const TypeSignature& typesignature);
        [[nodiscard]] size_t compute_header_unaware_inline_union_memory_footprint(const InlineUnion& inline_union);
        [[nodiscard]] size_t compute_header_unaware_named_union_memory_footprint(const UnionDefinition& inline_union);
        [[nodiscard]] size_t compute_header_unaware_customtype_memory_footprint(const CustomType& custom_type);

    private:
        ProgramRepresentation& program_representation;
        llvm::LLVMContext& context;
        llvm::Module& llvm_module;
        std::unordered_map<std::string, llvm::Type*> llvm_type_definitions;
};