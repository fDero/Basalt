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
            std::optional<TypeSignature> expected_return_type,
            llvm::LLVMContext& context,
            llvm::Module& llvm_module,
            llvm::Function* current_function,
            llvm::BasicBlock* loop_entry_block = nullptr,
            llvm::BasicBlock* loop_exit_block = nullptr
        );

        llvm::BasicBlock* translate_statement_to_llvm(llvm::BasicBlock* block, const Statement& statement);
        llvm::BasicBlock* translate_whole_codeblock_to_llvm(llvm::BasicBlock* block, const std::vector<Statement>& codeblock);
        llvm::BasicBlock* translate_conditional_to_llvm(llvm::BasicBlock* block, const Conditional& conditional);
        llvm::BasicBlock* translate_while_loop_to_llvm(llvm::BasicBlock* block, const WhileLoop& while_loop);
        llvm::BasicBlock* translate_until_loop_to_llvm(llvm::BasicBlock* block, const UntilLoop& until_loop);
        llvm::BasicBlock* translate_return_statement_to_llvm(llvm::BasicBlock* block, const Return& return_statement);
        llvm::BasicBlock* translate_break_statement_to_llvm(llvm::BasicBlock* block, const Break& break_statement);
        llvm::BasicBlock* translate_continue_statement_to_llvm(llvm::BasicBlock* block, const Continue& continue_statement);
        llvm::BasicBlock* translate_variable_declaration_to_llvm(llvm::BasicBlock* block, const VariableDeclaration& variable_declaration);
        llvm::BasicBlock* translate_constant_declaration_to_llvm(llvm::BasicBlock* block, const ConstDeclaration& const_declaration);
        llvm::BasicBlock* translate_assignment_to_llvm(llvm::BasicBlock* block, const Assignment& assignment);
        llvm::BasicBlock* translate_void_function_call_to_llvm(llvm::BasicBlock* block, const FunctionCall& function_call);
        llvm::BasicBlock* translate_assignment_to_llvm(llvm::BasicBlock* block, const Expression& target, const Expression& source);

        [[nodiscard]] TranslatedExpression translate_expression_to_llvm(llvm::BasicBlock* block, const Expression& expression);
        [[nodiscard]] TranslatedExpression translate_expr_function_call_to_llvm(llvm::BasicBlock* block, const FunctionCall& expr);
        [[nodiscard]] TranslatedExpression translate_array_literal_to_llvm(llvm::BasicBlock* block, const ArrayLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_binary_operator_to_llvm(llvm::BasicBlock* block, const BinaryOperator& expr);
        [[nodiscard]] TranslatedExpression translate_boolean_literal_to_llvm(llvm::BasicBlock* block, const BoolLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_character_literal_to_llvm(llvm::BasicBlock* block, const CharLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_floating_literal_to_llvm(llvm::BasicBlock* block, const FloatLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_identifier_to_llvm(llvm::BasicBlock* block, const Identifier& expr);
        [[nodiscard]] TranslatedExpression translate_integer_literal_to_llvm(llvm::BasicBlock* block, const IntLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_string_literal_to_llvm(llvm::BasicBlock* block, const StringLiteral& expr);
        [[nodiscard]] TranslatedExpression translate_type_operator_to_llvm(llvm::BasicBlock* block, const TypeOperator& expr);
        [[nodiscard]] TranslatedExpression translate_unary_operator_to_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);

        [[nodiscard]] TranslatedExpression translate_square_bracket_access_to_llvm(llvm::BasicBlock* block, const SquareBracketsAccess& expr);
        [[nodiscard]] TranslatedExpression translate_square_bracket_access_from_array_to_llvm(llvm::BasicBlock* block, const SquareBracketsAccess& expr);
        [[nodiscard]] TranslatedExpression translate_square_bracket_access_from_slice_to_llvm(llvm::BasicBlock* block, const SquareBracketsAccess& expr);
        [[nodiscard]] TranslatedExpression translate_dot_member_access_to_llvm(llvm::BasicBlock* block, const DotMemberAccess& expr);

        [[nodiscard]] TranslatedExpression translate_is_operator_to_llvm(llvm::BasicBlock* block, const TypeOperator& expr);
        [[nodiscard]] TranslatedExpression translate_as_operator_to_llvm(llvm::BasicBlock* block, const TypeOperator& expr);

        [[nodiscard]] TranslatedExpression translate_plus_binary_operator_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op); 
        [[nodiscard]] TranslatedExpression translate_minus_binary_operator_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);
        [[nodiscard]] TranslatedExpression translate_mul_binary_operator_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);  
        [[nodiscard]] TranslatedExpression translate_div_binary_operator_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);  
        [[nodiscard]] TranslatedExpression translate_mod_binary_operator_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);  
        [[nodiscard]] TranslatedExpression translate_LT_then_comparison_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);   
        [[nodiscard]] TranslatedExpression translate_GT_comparison_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_LE_comparison_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_GE_comparison_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_EQ_comparison_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_NE_comparison_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);        
        [[nodiscard]] TranslatedExpression translate_boolean_and_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);          
        [[nodiscard]] TranslatedExpression translate_boolean_or_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);           
        [[nodiscard]] TranslatedExpression translate_boolean_xor_to_llvm(llvm::BasicBlock* block, const BinaryOperator& binary_op);  

        [[nodiscard]] TranslatedExpression translate_boolean_not_to_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);
        [[nodiscard]] TranslatedExpression translate_minus_sign_to_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);
        [[nodiscard]] TranslatedExpression translate_plus_sign_to_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);
        [[nodiscard]] TranslatedExpression translate_increment_prefix_to_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);
        [[nodiscard]] TranslatedExpression translate_decrement_prefix_to_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);
        [[nodiscard]] TranslatedExpression translate_ptr_dereference_to_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);
        [[nodiscard]] TranslatedExpression translate_addressof_to_llvm(llvm::BasicBlock* block, const UnaryOperator& expr);

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
        std::optional<TypeSignature> expected_return_type;

        llvm::LLVMContext& context;
        llvm::Module& llvm_module;
        llvm::BasicBlock* loop_entry_block;
        llvm::BasicBlock* loop_exit_block;
        llvm::Function* current_function;
};