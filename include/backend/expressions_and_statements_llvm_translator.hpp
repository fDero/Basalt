//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <functional>

#include "core/program_representation.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "language/definitions.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class ExpressionsAndStatementsLLVMTranslator {
    public:
        ExpressionsAndStatementsLLVMTranslator(
            ProgramRepresentation& program_representation, 
            TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
            CallableCodeBlocksLLVMTranslator& callable_codeblocks_llvm_translator,
            ScopeContext& scope_context,
            llvm::LLVMContext& context,
            llvm::IRBuilder<>& builder,
            llvm::Function* current_function,
            llvm::BasicBlock* loop_entry_block = nullptr,
            llvm::BasicBlock* loop_exit_block = nullptr
        );

        void translate_whole_block_into_llvm(const std::vector<Statement>& statements);

        void translate_statement_into_llvm(const Statement& statement);
        void translate_conditional_into_llvm(const Conditional& conditional);
        void translate_while_loop_into_llvm(const WhileLoop& while_loop);
        void translate_until_loop_into_llvm(const UntilLoop& until_loop);
        void translate_return_statement_into_llvm(const Return& return_statement);
        void translate_break_statement_into_llvm(const Break& break_statement);
        void translate_continue_statement_into_llvm(const Continue& continue_statement);
        void translate_variable_declaration_into_llvm(const VariableDeclaration& variable_declaration);
        void translate_constant_declaration_into_llvm(const ConstDeclaration& const_declaration);
        void translate_assignment_into_llvm(const Assignment& assignment);
        void translate_void_function_call_into_llvm(const FunctionCall& function_call);
        
        [[nodiscard]] llvm::Value* translate_expression_into_llvm(const Expression& expression);
        [[nodiscard]] llvm::Value* translate_expr_function_call_into_llvm(const FunctionCall& expr);
        [[nodiscard]] llvm::Value* translate_array_literal_into_llvm(const ArrayLiteral& expr);
        [[nodiscard]] llvm::Value* translate_binary_operator_into_llvm(const BinaryOperator& expr);
        [[nodiscard]] llvm::Value* translate_boolean_literal_into_llvm(const BoolLiteral& expr);
        [[nodiscard]] llvm::Value* translate_character_literal_into_llvm(const CharLiteral& expr);
        [[nodiscard]] llvm::Value* translate_floating_literal_into_llvm(const FloatLiteral& expr);
        [[nodiscard]] llvm::Value* translate_identifier_into_llvm(const Identifier& expr);
        [[nodiscard]] llvm::Value* translate_integer_literal_into_llvm(const IntLiteral& expr);
        [[nodiscard]] llvm::Value* translate_string_literal_into_llvm(const StringLiteral& expr);
        [[nodiscard]] llvm::Value* translate_type_operator_into_llvm(const TypeOperator& expr);
        [[nodiscard]] llvm::Value* translate_unary_operator_into_llvm(const UnaryOperator& expr);

        [[nodiscard]] llvm::Value* translate_is_operator_into_llvm(const TypeOperator& expr);
        [[nodiscard]] llvm::Value* translate_as_operator_into_llvm(const TypeOperator& expr);
        [[nodiscard]] llvm::Value* translate_pow_operator_into_llvm(const BinaryOperator& expr);

    protected:
        [[nodiscard]] ExpressionsAndStatementsLLVMTranslator create_translator_for_nested_loop(
            llvm::BasicBlock* entry_block,
            llvm::BasicBlock* exit_block
        );

        [[nodiscard]] ExpressionsAndStatementsLLVMTranslator create_translator_for_nested_conditional();

    private:
        ProgramRepresentation& program_representation;
        TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator;
        CallableCodeBlocksLLVMTranslator& callable_codeblocks_llvm_translator;
        ScopeContext& scope_context;
        llvm::LLVMContext& context;
        llvm::IRBuilder<>& builder;
        llvm::BasicBlock* loop_entry_block;
        llvm::BasicBlock* loop_exit_block;
        llvm::Function* current_function;
};