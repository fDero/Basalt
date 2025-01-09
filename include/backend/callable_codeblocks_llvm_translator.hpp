//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <functional>

#include "core/program_representation.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "language/definitions.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class CallableCodeBlocksLLVMTranslator {
    public:
        CallableCodeBlocksLLVMTranslator(
            ProgramRepresentation& program_representation, 
            TypeDefinitionsLLVMTranslator& type,
            llvm::LLVMContext& context,
            llvm::Module& llvm_module
        );

        llvm::Function* translate_callable_code_block_into_llvm(const CallableCodeBlock& callable_code_block) {
            throw std::runtime_error("not implemented yet");
        }

        llvm::Function* translate_pow_builtin_operator_as_llvm_function(const BinaryOperator& binary_operator) {
            throw std::runtime_error("not implemented yet");
        }

        llvm::Function* translate_is_builtin_operator_as_llvm_function(const TypeOperator& type_operator) {
            throw std::runtime_error("not implemented yet");
        }
};