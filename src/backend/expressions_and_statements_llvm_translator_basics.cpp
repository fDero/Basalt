//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"

using TranslatedExpression = ExpressionsAndStatementsLLVMTranslator::TranslatedExpression;

ExpressionsAndStatementsLLVMTranslator::ExpressionsAndStatementsLLVMTranslator(
    ProgramRepresentation& program_representation, 
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
    CallableCodeBlocksLLVMTranslator& callable_codeblocks_llvm_translator,
    TranslationAwareScopeContext scope_context,
    llvm::LLVMContext& context,
    llvm::Function* current_function,
    llvm::BasicBlock* entry_block,
    llvm::BasicBlock* exit_block
)
    : program_representation(program_representation)
    , type_definitions_llvm_translator(type_definitions_llvm_translator)
    , callable_codeblocks_llvm_translator(callable_codeblocks_llvm_translator)
    , scope_context(scope_context)
    , context(context)
    , loop_entry_block(entry_block)
    , loop_exit_block(exit_block)
    , current_function(current_function)
{ }

[[nodiscard]] ExpressionsAndStatementsLLVMTranslator 
ExpressionsAndStatementsLLVMTranslator::create_translator_for_nested_loop(
    llvm::BasicBlock* new_entry_block,
    llvm::BasicBlock* new_exit_block
){
    return ExpressionsAndStatementsLLVMTranslator(
        program_representation, 
        type_definitions_llvm_translator, 
        callable_codeblocks_llvm_translator, 
        scope_context.create_nested_scope(), 
        context,   
        current_function,
        new_entry_block, 
        new_exit_block
    );
}

[[nodiscard]] ExpressionsAndStatementsLLVMTranslator 
ExpressionsAndStatementsLLVMTranslator::create_translator_for_nested_conditional(){
    return create_translator_for_nested_loop(loop_entry_block, loop_exit_block);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_expr_function_call_into_llvm(
    llvm::BasicBlock* block,
    const FunctionCall& fcall
) {
    CallableCodeBlock ccb = program_representation.resolve_function_call(fcall, scope_context.raw_scope_context);
    llvm::Function* llvm_function = callable_codeblocks_llvm_translator.translate_callable_code_block_into_llvm(ccb);
    llvm::IRBuilder<> builder(block);
    return builder.CreateCall(llvm_function);
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_void_function_call_into_llvm(
    llvm::BasicBlock* block,
    const FunctionCall& function_call
) {
    std::ignore = translate_expr_function_call_into_llvm(block, function_call);
    return block;
}