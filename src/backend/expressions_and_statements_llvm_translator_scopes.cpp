//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_manipulations_llvm_translator.hpp"
#include "backend/llvm_wrappers.hpp"
#include "errors/internal_errors.hpp"

static void translate_block_to_llvm_with_final_jump(
    llvm::BasicBlock* block, 
    const std::vector<Statement>& codeblock, 
    ExpressionsAndStatementsLLVMTranslator& translator,
    llvm::BasicBlock* final_jump_block
) {
    for (const Statement& statement : codeblock) {
        block = translator.translate_statement_to_llvm(block, statement);
    }
    if (block->empty() || !block->getTerminator()) {
        llvm::IRBuilder<> builder(block);
        builder.CreateBr(final_jump_block);
    }
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_whole_codeblock_to_llvm(
    llvm::BasicBlock* block,
    const std::vector<Statement>& codeblock
) {
    for (const Statement& statement : codeblock) {
        block = translate_statement_to_llvm(block, statement);
    }
    return block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_conditional_to_llvm(
    llvm::BasicBlock* current_block,
    const Conditional& conditional
) {
    std::string unique_conditional_id = conditional.as_debug_informations_aware_entity().unique_string_id();
    auto if_cond_block = createBlockAfter(context, "if:cond@" + unique_conditional_id, current_block);
    auto if_then_block = createBlockAfter(context, "if:then@" + unique_conditional_id, if_cond_block);
    auto if_else_block = createBlockAfter(context, "if:else@" + unique_conditional_id, if_then_block);
    auto if_exit_block = createBlockAfter(context, "if:exit@" + unique_conditional_id, if_else_block);

    llvm::IRBuilder<> current_builder(current_block);
    current_builder.CreateBr(if_cond_block);

    llvm::IRBuilder<> cond_builder(if_cond_block);
    llvm::Value* condition = translate_expression_to_llvm(if_cond_block, conditional.condition).value;
    cond_builder.CreateCondBr(condition, if_then_block, if_else_block);
    
    auto then_translator = create_translator_for_nested_conditional();
    translate_block_to_llvm_with_final_jump(if_then_block, conditional.then_branch, then_translator, if_exit_block);

    auto else_translator = create_translator_for_nested_conditional();
    translate_block_to_llvm_with_final_jump(if_else_block, conditional.else_branch, else_translator, if_exit_block);

    return if_exit_block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_while_loop_to_llvm(
    llvm::BasicBlock* current_block,
    const WhileLoop& while_loop
) {
    std::string unique_while_id = while_loop.as_debug_informations_aware_entity().unique_string_id();
    auto while_cond_block = createBlockAfter(context, "while:cond@" + unique_while_id, current_block);
    auto while_body_block = createBlockAfter(context, "while:body@" + unique_while_id, while_cond_block);
    auto while_exit_block = createBlockAfter(context, "while:exit@" + unique_while_id, while_body_block);

    llvm::IRBuilder<> current_builder(current_block);
    current_builder.CreateBr(while_cond_block);

    llvm::IRBuilder<> cond_builder(while_cond_block);
    llvm::Value* condition = translate_expression_to_llvm(while_cond_block, while_loop.condition).value;
    cond_builder.CreateCondBr(condition, while_body_block, while_exit_block);

    auto body_translator = create_translator_for_nested_loop(while_cond_block, while_exit_block);
    translate_block_to_llvm_with_final_jump(while_body_block, while_loop.loop_body, body_translator, while_cond_block);

    return while_exit_block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_until_loop_to_llvm(
    llvm::BasicBlock* current_block,
    const UntilLoop& until_loop
) {
    std::string unique_until_id = until_loop.as_debug_informations_aware_entity().unique_string_id();
    auto until_body_block = createBlockAfter(context, "until:body@" + unique_until_id, current_block);
    auto until_cond_block = createBlockAfter(context, "until:cond@" + unique_until_id, until_body_block);
    auto until_exit_block = createBlockAfter(context, "until:exit@" + unique_until_id, until_cond_block);
    
    llvm::IRBuilder<> current_builder(current_block);
    current_builder.CreateBr(until_body_block);

    auto body_translator = create_translator_for_nested_loop(until_body_block, until_exit_block);
    translate_block_to_llvm_with_final_jump(until_body_block, until_loop.loop_body, body_translator, until_cond_block);

    llvm::IRBuilder<> cond_builder(until_cond_block);
    llvm::Value* condition = translate_expression_to_llvm(until_cond_block, until_loop.condition).value;
    llvm::Value* negated_condition = cond_builder.CreateNot(condition);
    cond_builder.CreateCondBr(negated_condition, until_body_block, until_exit_block);

    return until_exit_block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_return_statement_to_llvm(
    llvm::BasicBlock* block,
    const Return& return_statement
) {
    TypeManipulationsLLVMTranslator type_manipulations_llvm_translator(program_representation, type_definitions_llvm_translator);
    llvm::IRBuilder<> builder(block);
    if (return_statement.return_value.has_value()) {
        const Expression& ret = return_statement.return_value.value();
        TranslatedExpression translated_ret = translate_expression_to_llvm(block, ret);
        auto provided_ret_type_opt = program_representation.resolve_expression_type(ret, scope_context.raw_scope_context);
        assert_type_deduction_success_in_backend_layer(provided_ret_type_opt.has_value());
        TypeSignature provided_ret_type = provided_ret_type_opt.value();
        TranslatedExpression casted_ret_expr = type_manipulations_llvm_translator.cast_translated_expression_to_another_type_in_llvm(
            block,
            translated_ret,
            provided_ret_type,
            expected_return_type.value()
        );
        builder.CreateRet(casted_ret_expr.value);
        return block;
    }   
    builder.CreateRetVoid();
    return block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_break_statement_to_llvm(
    llvm::BasicBlock* block,
    const Break& break_statement
) {
    llvm::IRBuilder<> builder(block);
    builder.CreateBr(loop_exit_block);
    return block;
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_continue_statement_to_llvm(
    llvm::BasicBlock* block,
    const Continue& continue_statement
) {
    llvm::IRBuilder<> builder(block);
    builder.CreateBr(loop_entry_block);
    return block;
}