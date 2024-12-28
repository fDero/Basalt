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

class FunctionDefinitionsLLVMTranslator {
    public:
        FunctionDefinitionsLLVMTranslator(
            TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
            ProgramRepresentation& program_representation, 
            llvm::LLVMContext& context,
            llvm::Module& llvm_module
        );
        
        [[nodiscard]] llvm::Type* translate_return_type_to_llvm_type(const FunctionDefinition::Ref& func_def);
        [[nodiscard]] std::vector<llvm::Type*> translate_arguments_to_llvm_types(const FunctionDefinition::Ref& func_def);

        void translate_function_definition_to_llvm(
            const FunctionDefinition::Ref& func_def
        );
        
        void translate_function_body_to_llvm(
            const FunctionDefinition::Ref& function_definition,
            llvm::Function* function
        );

    private:
        TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator;
        ProgramRepresentation& program_representation;
        llvm::LLVMContext& context;
        llvm::Module& llvm_module;
        std::unordered_map<std::string, llvm::Function*> llvm_function_definitions;
};