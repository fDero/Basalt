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

class TypeOperatorsLLVMTranslator {

    public:
        TypeOperatorsLLVMTranslator(
            ProgramRepresentation& program_representation, 
            TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator
        )
            : program_representation(program_representation)
            , type_definitions_llvm_translator(type_definitions_llvm_translator)
        { }

        [[nodiscard]] TranslatedExpression translate_is_operator_to_llvm_value(
            llvm::BasicBlock* block,
            TranslatedExpression union_expression,
            const TypeSignature& type_to_check
        ) {
            llvm::IRBuilder<> builder(block);
            llvm::Value* union_type_info_address = builder.CreateGEP(union_expression.address, {0, 0});
            llvm::Value* union_type_info_value = builder.CreateLoad(union_type_info_address);
            llvm::GlobalVariable* expected_type_info = type_definitions_llvm_translator.fetch_type_info(type_to_check);
            return builder.CreateICmpEQ(union_type_info_value, expected_type_info);
        }

        [[nodiscard]] TranslatedExpression translate_as_operator_to_llvm_value(
            llvm::BasicBlock* block,
            TranslatedExpression union_expression,
            const TypeSignature& type_to_cast_to
        ) {
            if (program_representation.is_union(type_to_cast_to)) {
                return union_expression;
            }
            llvm::IRBuilder<> builder(block);
            llvm::Value* union_payload_address = builder.CreateGEP(union_expression.address, {0, 1});
            llvm::Value* union_payload_value = builder.CreateLoad(union_payload_address);
            return TranslatedExpression(union_payload_value, union_expression.address);
        }

    private:
        ProgramRepresentation& program_representation;
        TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator;
};