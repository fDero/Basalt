//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/program_representation.hpp"
#include "language/definitions.hpp"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>

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
        [[nodiscard]] llvm::Type* translate_return_type_to_llvm_type(const std::optional<TypeSignature>& ret);
        [[nodiscard]] llvm::Type* translate_primitive_type_to_llvm(const PrimitiveType& primitive_type);
        [[nodiscard]] llvm::Type* translate_array_type_to_llvm_type(const ArrayType& array_type);
        [[nodiscard]] llvm::Type* translate_slice_type_to_llvm_type(const SliceType& slice_type);
        [[nodiscard]] llvm::Type* translate_pointer_type_to_llvm_type(const PointerType& pointer_type);

        [[nodiscard]] llvm::Type* get_int_llvm_type();
        [[nodiscard]] llvm::Type* get_float_llvm_type();
        [[nodiscard]] llvm::Type* get_char_llvm_type();
        [[nodiscard]] llvm::Type* get_bool_llvm_type();
        [[nodiscard]] llvm::Type* get_string_llvm_type();
        [[nodiscard]] llvm::Type* get_raw_string_llvm_type();

        [[nodiscard]] size_t compute_header_unaware_typesignature_memory_footprint(const TypeSignature& typesignature);
        [[nodiscard]] size_t compute_header_unaware_inline_union_memory_footprint(const InlineUnion& inline_union);
        [[nodiscard]] size_t compute_header_unaware_named_union_memory_footprint(const UnionDefinition& inline_union);
        [[nodiscard]] size_t compute_header_unaware_customtype_memory_footprint(const CustomType& custom_type);

        [[nodiscard]] std::vector<llvm::Type*> translate_all_types_to_llvm_types(const std::vector<TypeSignature>& types);

        [[nodiscard]] llvm::GlobalVariable* fetch_type_info(const TypeSignature& type_signature);
        [[nodiscard]] std::vector<llvm::GlobalVariable*> fetch_all_type_infos_for_non_union_compatible_types(const TypeSignature& type_signature);

    protected:
        [[nodiscard]] llvm::Type* translate_union_to_llvm_type(
            const std::vector<TypeSignature>& alternatives,
            const std::string& fully_qualified_name
        );

    private:
        ProgramRepresentation& program_representation;
        llvm::LLVMContext& context;
        llvm::Module& llvm_module;

        std::unordered_map<std::string, llvm::Type*> llvm_type_definitions;
        std::unordered_map<std::string, llvm::GlobalVariable*> llvm_exact_type_infos;
        std::unordered_map<std::string, std::vector<llvm::GlobalVariable*>> llvm_compatible_type_infos;
};