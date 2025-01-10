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

        llvm::Function* translate_callable_code_block_into_llvm(const CallableCodeBlock& callable_code_block);

        llvm::Function* translate_common_feature_adoption_plan_into_llvm(
            const CommonFeatureAdoptionPlanDescriptor& cfa_plan, 
            llvm::Function* llvm_function
        );

        llvm::Function* translate_function_definition_into_llvm(
            const FunctionDefinition::Ref& function_definition, 
            llvm::Function* llvm_function
        );

        ExpressionsAndStatementsLLVMTranslator get_function_body_translator(
            TranslationAwareScopeContext scope_context,
            llvm::Function* llvm_function,
            llvm::IRBuilder<>& llvm_builder
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

    private:
        ProgramRepresentation& program_representation;
        TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator;
        std::map<std::string, llvm::Function*> llvm_functions;
        llvm::LLVMContext& llvm_context;
        llvm::Module& llvm_module;
};