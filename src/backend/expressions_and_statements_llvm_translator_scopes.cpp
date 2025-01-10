//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

void ExpressionsAndStatementsLLVMTranslator::translate_whole_codeblock_into_llvm(const std::vector<Statement>& codeblock) {
    for (const Statement& statement : codeblock) {
        translate_statement_into_llvm(statement);
    }
}

void ExpressionsAndStatementsLLVMTranslator::translate_conditional_into_llvm(const Conditional& conditional) {
    std::string unique_conditional_id = conditional.as_debug_informations_aware_entity().unique_string_id();
    auto if_cond_block = llvm::BasicBlock::Create(context, "if:cond@" + unique_conditional_id, current_function);
    auto if_then_block = llvm::BasicBlock::Create(context, "if:then@" + unique_conditional_id, current_function);
    auto if_else_block = llvm::BasicBlock::Create(context, "if:else@" + unique_conditional_id, current_function);
    auto if_exit_block = llvm::BasicBlock::Create(context, "if:exit@" + unique_conditional_id, current_function);
    builder.SetInsertPoint(if_cond_block);
    llvm::Value* condition = translate_expression_into_llvm(conditional.condition).value;
    builder.CreateCondBr(condition, if_then_block, if_else_block);
    builder.SetInsertPoint(if_then_block);
    auto then_translator = create_translator_for_nested_conditional();
    then_translator.translate_whole_codeblock_into_llvm(conditional.then_branch);
    builder.SetInsertPoint(if_else_block);
    auto else_translator = create_translator_for_nested_conditional();
    else_translator.translate_whole_codeblock_into_llvm(conditional.else_branch);
    builder.SetInsertPoint(if_exit_block);
}

void ExpressionsAndStatementsLLVMTranslator::translate_while_loop_into_llvm(const WhileLoop& while_loop) {
    std::string unique_while_id = while_loop.as_debug_informations_aware_entity().unique_string_id();
    auto while_cond_block = llvm::BasicBlock::Create(context, "while:cond@" + unique_while_id, current_function);
    auto while_body_block = llvm::BasicBlock::Create(context, "while:body@" + unique_while_id, current_function);
    auto while_exit_block = llvm::BasicBlock::Create(context, "while:exit@" + unique_while_id, current_function);
    builder.SetInsertPoint(while_cond_block);
    llvm::Value* condition = translate_expression_into_llvm(while_loop.condition).value;
    builder.CreateCondBr(condition, while_body_block, while_exit_block);
    builder.SetInsertPoint(while_body_block);
    auto body_translator = create_translator_for_nested_loop(while_cond_block, while_exit_block);
    body_translator.translate_whole_codeblock_into_llvm(while_loop.loop_body);
    builder.CreateBr(while_cond_block);
    builder.SetInsertPoint(while_exit_block);
}

void ExpressionsAndStatementsLLVMTranslator::translate_until_loop_into_llvm(const UntilLoop& until_loop) {
    std::string unique_until_id = until_loop.as_debug_informations_aware_entity().unique_string_id();
    auto until_body_block = llvm::BasicBlock::Create(context, "until:body@" + unique_until_id, current_function);
    auto until_cond_block = llvm::BasicBlock::Create(context, "until:cond@" + unique_until_id, current_function);
    auto until_exit_block = llvm::BasicBlock::Create(context, "until:exit@" + unique_until_id, current_function);
    builder.SetInsertPoint(until_body_block);
    auto body_translator = create_translator_for_nested_loop(until_body_block, until_exit_block);
    body_translator.translate_whole_codeblock_into_llvm(until_loop.loop_body);
    builder.SetInsertPoint(until_cond_block);
    llvm::Value* condition = translate_expression_into_llvm(until_loop.condition).value;
    llvm::Value* negated_condition = builder.CreateNot(condition);
    builder.CreateCondBr(negated_condition, until_body_block, until_exit_block);
    builder.SetInsertPoint(until_exit_block);
}

void ExpressionsAndStatementsLLVMTranslator::translate_return_statement_into_llvm(const Return& return_statement) {
    if (return_statement.return_value.has_value()) {
        llvm::Value* returned_value = translate_expression_into_llvm(return_statement.return_value.value()).value;
        builder.CreateRet(returned_value);
    } else {
        builder.CreateRetVoid();
    }
}

void ExpressionsAndStatementsLLVMTranslator::translate_break_statement_into_llvm(const Break& break_statement) {
    builder.CreateBr(loop_exit_block);
}

void ExpressionsAndStatementsLLVMTranslator::translate_continue_statement_into_llvm(const Continue& continue_statement) {
    builder.CreateBr(loop_entry_block);
}