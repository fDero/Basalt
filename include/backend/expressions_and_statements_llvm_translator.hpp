//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <functional>
#include <memory>

#include "misc/forward_declarations.hpp"
#include "language/definitions.hpp"
#include "core/program_representation.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "backend/translation_aware_scope_context.hpp"
#include "backend/translated_expression.hpp"


#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class ExpressionsAndStatementsLLVMTranslator {
    public:
        ExpressionsAndStatementsLLVMTranslator(
            ProgramRepresentation& program_representation, 
            TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
            CallableCodeBlocksLLVMTranslator& callable_codeblocks_llvm_translator,
            TranslationAwareScopeContext scope_context,
            llvm::LLVMContext& context,
            llvm::Function* current_function,
            llvm::BasicBlock* loop_entry_block = nullptr,
            llvm::BasicBlock* loop_exit_block = nullptr
        );

        llvm::BasicBlock* translate_statement_into_llvm(llvm::BasicBlock* block, const Statement& statement);
        llvm::BasicBlock* translate_whole_codeblock_into_llvm(llvm::BasicBlock* block, const std::vector<Statement>& codeblock);
        llvm::BasicBlock* translate_conditional_into_llvm(llvm::BasicBlock* block, const Conditional& conditional);
        llvm::BasicBlock* translate_while_loop_into_llvm(llvm::BasicBlock* block, const WhileLoop& while_loop);
        llvm::BasicBlock* translate_until_loop_into_llvm(llvm::BasicBlock* block, const UntilLoop& until_loop);
        llvm::BasicBlock* translate_return_statement_into_llvm(llvm::BasicBlock* block, const Return& return_statement);
        llvm::BasicBlock* translate_break_statement_into_llvm(llvm::BasicBlock* block, const Break& break_statement);
        llvm::BasicBlock* translate_continue_statement_into_llvm(llvm::BasicBlock* block, const Continue& continue_statement);
        llvm::BasicBlock* translate_variable_declaration_into_llvm(llvm::BasicBlock* block, const VariableDeclaration& variable_declaration);
        llvm::BasicBlock* translate_constant_declaration_into_llvm(llvm::BasicBlock* block, const ConstDeclaration& const_declaration);
        llvm::BasicBlock* translate_assignment_into_llvm(llvm::BasicBlock* block, const Assignment& assignment);
        llvm::BasicBlock* translate_void_function_call_into_llvm(llvm::BasicBlock* block, const FunctionCall& function_call);
        llvm::BasicBlock* translate_assignment_into_llvm(llvm::BasicBlock* block, const Expression& target, const Expression& source);

        [[nodiscard]] TranslatedExpression translate_expression_into_llvm(llvm::BasicBlock* block, const Expression& expression);
        [[nodiscard]] TranslatedExpression translate_expr_function_call_into_llvm(llvm::BasicBlock* block, const FunctionCall& expr);
        [[nodiscard]] TranslatedExpression translate_array_literal_into_llvm(llvm::BasicBlock* block, const ArrayLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_binary_operator_into_llvm(llvm::BasicBlock* block, const BinaryOperator& expr);
        [[nodiscard]] TranslatedExpression translate_boolean_literal_into_llvm(llvm::BasicBlock* block, const BoolLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_character_literal_into_llvm(llvm::BasicBlock* block, const CharLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_floating_literal_into_llvm(llvm::BasicBlock* block, const FloatLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_identifier_into_llvm(llvm::BasicBlock* block, const Identifier& expr);
        [[nodiscard]] TranslatedExpression translate_integer_literal_into_llvm(llvm::BasicBlock* block, const IntLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_string_literal_into_llvm(llvm::BasicBlock* block, const StringLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_type_operator_into_llvm(llvm::BasicBlock* block, const TypeOperator& expr);
        [[nodiscard]] TranslatedExpression translate_unary_operator_into_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);

        [[nodiscard]] TranslatedExpression translate_square_bracket_access_into_llvm(llvm::BasicBlock* block, const SquareBracketsAccess& expr);
        [[nodiscard]] TranslatedExpression translate_square_bracket_access_from_array_into_llvm(llvm::BasicBlock* block, const SquareBracketsAccess& expr);
        [[nodiscard]] TranslatedExpression translate_square_bracket_access_from_slice_into_llvm(llvm::BasicBlock* block, const SquareBracketsAccess& expr);
        [[nodiscard]] TranslatedExpression translate_dot_member_access_into_llvm(llvm::BasicBlock* block, const DotMemberAccess& expr);

        [[nodiscard]] TranslatedExpression translate_is_operator_into_llvm(llvm::BasicBlock* block, const TypeOperator& expr);
        [[nodiscard]] TranslatedExpression translate_as_operator_into_llvm(llvm::BasicBlock* block, const TypeOperator& expr);

        [[nodiscard]] TranslatedExpression translate_plus_binary_operator(llvm::BasicBlock* block, const BinaryOperator& binary_op); 
        [[nodiscard]] TranslatedExpression translate_minus_binary_operator(llvm::BasicBlock* block, const BinaryOperator& binary_op);
        [[nodiscard]] TranslatedExpression translate_mul_binary_operator(llvm::BasicBlock* block, const BinaryOperator& binary_op);  
        [[nodiscard]] TranslatedExpression translate_div_binary_operator(llvm::BasicBlock* block, const BinaryOperator& binary_op);  
        [[nodiscard]] TranslatedExpression translate_mod_binary_operator(llvm::BasicBlock* block, const BinaryOperator& binary_op);  
        [[nodiscard]] TranslatedExpression translate_LT_then_comparison(llvm::BasicBlock* block, const BinaryOperator& binary_op);   
        [[nodiscard]] TranslatedExpression translate_GT_comparison(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_LE_comparison(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_GE_comparison(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_EQ_comparison(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_NE_comparison(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_boolean_and(llvm::BasicBlock* block, const BinaryOperator& binary_op);          
        [[nodiscard]] TranslatedExpression translate_boolean_or(llvm::BasicBlock* block, const BinaryOperator& binary_op);           
        [[nodiscard]] TranslatedExpression translate_boolean_xor(llvm::BasicBlock* block, const BinaryOperator& binary_op);  

    protected:        
        [[nodiscard]] ExpressionsAndStatementsLLVMTranslator create_translator_for_nested_conditional();

        [[nodiscard]] ExpressionsAndStatementsLLVMTranslator create_translator_for_nested_loop(
            llvm::BasicBlock* new_loop_entry_block,
            llvm::BasicBlock* new_loop_exit_block
        );

    private:
        ProgramRepresentation& program_representation;
        TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator;
        CallableCodeBlocksLLVMTranslator& callable_codeblocks_llvm_translator;
        TranslationAwareScopeContext scope_context;
        llvm::LLVMContext& context;
        llvm::BasicBlock* loop_entry_block;
        llvm::BasicBlock* loop_exit_block;
        llvm::Function* current_function;
};