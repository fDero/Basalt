//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#pragma once

#include <functional>

#include "core/program_representation.hpp"
#include "language/definitions.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class StatementLLVMTranslator {
    public:
        StatementLLVMTranslator(
            ProgramRepresentation& program_representation, 
            llvm::BasicBlock* entry_code_block,
            llvm::BasicBlock* exit_code_block
        );
        
        void translate_statement_into_llvm(const Statement& statement);

        void translate_conditional_into_llvm(const Conditional& conditional);
        void translate_while_loop_into_llvm(const WhileLoop& while_loop);
        void translate_until_loop_into_llvm(const UntilLoop& until_loop);

        void translate_return_statement_into_llvm(const Return& return_statement);
        void translate_break_statement_into_llvm(const Break& break_statement);
        void translate_continue_statement_into_llvm(const Continue& continue_statement);

        void translate_function_call_into_llvm(const FunctionCall& function_call);
        void translate_variable_declaration_into_llvm(const VariableDeclaration& variable_declaration);
        void translate_constant_declaration_into_llvm(const ConstDeclaration& const_declaration);
        void translate_assignment_into_llvm(const Assignment& assignment);

    private:
        ProgramRepresentation& program_representation;
        llvm::BasicBlock* entry_code_block;
        llvm::BasicBlock* exit_code_block;
        llvm::IRBuilder<> llvm_builder;
};