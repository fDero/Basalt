//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <functional>

#include "core/program_representation.hpp"
#include "core/scope_context.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "language/definitions.hpp"
#include "backend/translation_aware_scope_context.hpp"
#include "backend/expressions_and_statements_llvm_translator.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class CallableCodeBlocksLLVMTranslator {
    public:
        CallableCodeBlocksLLVMTranslator(
            ProgramRepresentation& program_representation, 
            TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
            llvm::LLVMContext& llvm_context,
            llvm::Module& llvm_module
        );

        llvm::Function* translate_callable_code_block_into_llvm(
            const CallableCodeBlock& callable_code_block
        );

        llvm::Function* translate_cfa_descriptor_into_llvm(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor, 
            llvm::Function* llvm_function
        );

        llvm::Function* translate_function_definition_into_llvm(
            const FunctionDefinition::Ref& function_definition, 
            llvm::Function* llvm_function
        );

    protected:
        [[nodiscard]] ExpressionsAndStatementsLLVMTranslator get_function_body_translator(
            TranslationAwareScopeContext scope_context,
            llvm::Function* llvm_function,
            llvm::BasicBlock* function_entry_block,
            llvm::IRBuilder<>& llvm_builder
        );

        void translate_cfa_plan_into_llvm(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor, 
            const CommonFeatureAdoptionPlan& cfa_plan,
            llvm::Function* llvm_function,
            llvm::BasicBlock* block
        );

        void translate_cfa_recursive_adoption_into_llvm(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor, 
            const RecursiveAdoptionPlan& recursive_plan,
            llvm::Function* llvm_function,
            llvm::BasicBlock* block
        );

        void translate_cfa_direct_adoption_into_llvm(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
            const FunctionDefinition::Ref& selected_concrete_function,
            llvm::Function* llvm_function,
            llvm::BasicBlock* block
        );

    private:
        ProgramRepresentation& program_representation;
        TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator;
        std::map<std::string, llvm::Function*> llvm_functions;
        llvm::LLVMContext& llvm_context;
        llvm::Module& llvm_module;
};