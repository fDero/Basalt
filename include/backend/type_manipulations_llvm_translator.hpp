//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include "core/program_representation.hpp"
#include "backend/translated_expression.hpp"
#include "backend/type_definitions_llvm_translator.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>

class TypeManipulationsLLVMTranslator {

    public:
        TypeManipulationsLLVMTranslator(
            ProgramRepresentation& program_representation, 
            TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator
        );

        [[nodiscard]] TranslatedExpression test_concrete_type_of_union_in_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression union_expression,
            const TypeSignature& type_to_check
        );

        [[nodiscard]] TranslatedExpression cast_translated_expression_to_another_type_in_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression expression,
            const TypeSignature& expression_original_type,
            const TypeSignature& dest_type
        );

    protected:
        enum class CastStrategy {
            noop,
            union_to_union,
            union_to_alternative,
            alternative_to_union,
            array_pointer_to_slice,
            array_pointer_to_string,
            array_pointer_to_raw_string,
            slice_to_string,
            slice_to_raw_string,
            string_to_raw_string,
            array_to_array,
        };

        [[nodiscard]] CastStrategy compute_cast_strategy(
            const TypeSignature& original_type,
            const TypeSignature& dest_type
        );

        [[nodiscard]] TranslatedExpression cast_union_expression_to_another_union_type_in_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression union_expression,
            const TypeSignature& dest_type
        );

        [[nodiscard]] TranslatedExpression cast_union_expression_to_one_of_its_alternatives_in_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression union_expression,
            const TypeSignature& dest_type
        );

        [[nodiscard]] TranslatedExpression cast_simple_expression_to_union_in_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression simple_expression,
            const TypeSignature& soruce_type,
            const TypeSignature& dest_type
        );

        [[nodiscard]] TranslatedExpression cast_pointer_to_array_to_slice_in_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression pointer_to_array_expression,
            const TypeSignature& dest_type,
            const PointerType& array_type
        );

        [[nodiscard]] TranslatedExpression cast_slice_of_chars_to_string_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression slice_of_chars_expression
        );

        [[nodiscard]] TranslatedExpression cast_string_to_raw_string_in_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression string_expression
        );

        [[nodiscard]] TranslatedExpression cast_array_to_array_of_another_type_in_llvm(
            llvm::BasicBlock* block,
            TranslatedExpression array_expression,
            const ArrayType& source_array_type,
            const ArrayType& dest_array_type
        );

    private:
        ProgramRepresentation& program_representation;
        TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator;
};