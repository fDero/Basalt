//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <functional>

#include "core/program_representation.hpp"
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

        [[nodiscard]] llvm::Function* translate_pow_builtin_operator_as_llvm_function(
            const BinaryOperator& binary_operator
        );

        [[nodiscard]] llvm::Function* translate_is_builtin_operator_as_llvm_function(
            const TypeOperator& type_operator
        );

        [[nodiscard]] llvm::Function* translate_as_builtin_operator_as_llvm_function(
            const TypeOperator& type_operator
        );

    protected:
        ExpressionsAndStatementsLLVMTranslator get_function_body_translator(
            TranslationAwareScopeContext scope_context,
            llvm::Function* llvm_function,
            llvm::IRBuilder<>& llvm_builder
        );

        void translate_cfa_plan_into_llvm(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor, 
            const CommonFeatureAdoptionPlan& cfa_plan,
            const std::string& cfa_path_prefix, 
            llvm::Function* llvm_function,
            llvm::IRBuilder<>& llvm_builder
        );

        std::vector<llvm::BasicBlock*> create_necessary_cfa_cond_blocks(
            const RecursiveAdoptionPlan& recursive_plan,
            llvm::Function* llvm_function,
            const std::string& cfa_path_prefix
        );

        std::vector<llvm::BasicBlock*> create_necessary_cfa_run_blocks(
            const RecursiveAdoptionPlan& recursive_plan,
            llvm::Function* llvm_function,
            const std::string& cfa_path_prefix
        );

        void populate_cfa_cond_blocks(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
            const RecursiveAdoptionPlan& recursive_plan,
            const std::vector<llvm::BasicBlock*>& cond_blocks,
            const std::vector<llvm::BasicBlock*>& run_blocks,
            llvm::Function* llvm_function,
            llvm::IRBuilder<>& llvm_builder
        );

        void populate_cfa_run_blocks(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
            const RecursiveAdoptionPlan& recursive_plan,
            const std::string& cfa_path_prefix, 
            const std::vector<llvm::BasicBlock*>& run_blocks,
            llvm::Function* llvm_function,
            llvm::IRBuilder<>& llvm_builder
        );

        void handle_direct_cfa_adoption(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
            const FunctionDefinition::Ref& selected_concrete_function,
            llvm::Function* llvm_function,
            llvm::IRBuilder<>& llvm_builder
        );

    private:
        ProgramRepresentation& program_representation;
        TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator;
        std::map<std::string, llvm::Function*> llvm_functions;
        llvm::LLVMContext& llvm_context;
        llvm::Module& llvm_module;
};