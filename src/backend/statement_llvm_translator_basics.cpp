//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/statement_llvm_translator.hpp"

StatementLLVMTranslator::StatementLLVMTranslator(
    ProgramRepresentation& program_representation, 
    llvm::BasicBlock* entry_code_block,
    llvm::BasicBlock* exit_code_block
)
    : program_representation(program_representation)
    , entry_code_block(entry_code_block)
    , exit_code_block(exit_code_block)
    , llvm_builder(entry_code_block)
{ }

void StatementLLVMTranslator::translate_statement_into_llvm(
    const Statement& statement
){
    switch (statement.statement_kind()) {
        break; case StatementBody::Kind::assignment: translate_assignment_into_llvm(statement.get<Assignment>());
        break; case StatementBody::Kind::variable_declaration: translate_variable_declaration_into_llvm(statement.get<VariableDeclaration>());
        break; case StatementBody::Kind::const_declaration: translate_constant_declaration_into_llvm(statement.get<ConstDeclaration>());
        break; case StatementBody::Kind::function_call: translate_function_call_into_llvm(statement.get<FunctionCall>());
        break; case StatementBody::Kind::return_statement: translate_return_statement_into_llvm(statement.get<Return>());
        break; case StatementBody::Kind::break_statement: translate_break_statement_into_llvm(statement.get<Break>());
        break; case StatementBody::Kind::continue_statement: translate_continue_statement_into_llvm(statement.get<Continue>());
        break; case StatementBody::Kind::conditional: translate_conditional_into_llvm(statement.get<Conditional>());
        break; case StatementBody::Kind::while_loop: translate_while_loop_into_llvm(statement.get<WhileLoop>());
        break; case StatementBody::Kind::until_loop: translate_until_loop_into_llvm(statement.get<UntilLoop>());
    }
}