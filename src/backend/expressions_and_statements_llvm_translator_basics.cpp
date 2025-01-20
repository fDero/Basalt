//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/expressions_and_statements_llvm_translator.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_manipulations_llvm_translator.hpp"

ExpressionsAndStatementsLLVMTranslator::ExpressionsAndStatementsLLVMTranslator(
    ProgramRepresentation& program_representation, 
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
    CallableCodeBlocksLLVMTranslator& callable_codeblocks_llvm_translator,
    TranslationAwareScopeContext scope_context,
    std::optional<TypeSignature> expected_return_type,
    llvm::LLVMContext& context,
    llvm::Module& llvm_module,
    llvm::Function* current_function,
    llvm::BasicBlock* entry_block,
    llvm::BasicBlock* exit_block
)
    : program_representation(program_representation)
    , type_definitions_llvm_translator(type_definitions_llvm_translator)
    , callable_codeblocks_llvm_translator(callable_codeblocks_llvm_translator)
    , scope_context(scope_context)
    , expected_return_type(expected_return_type)
    , context(context)
    , llvm_module(llvm_module)
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
        expected_return_type,
        context,
        llvm_module,   
        current_function,
        new_entry_block, 
        new_exit_block
    );
}

[[nodiscard]] ExpressionsAndStatementsLLVMTranslator 
ExpressionsAndStatementsLLVMTranslator::create_translator_for_nested_conditional(){
    return create_translator_for_nested_loop(loop_entry_block, loop_exit_block);
}

TranslatedExpression ExpressionsAndStatementsLLVMTranslator::translate_expr_function_call_to_llvm(
    llvm::BasicBlock* block,
    const FunctionCall& fcall
) {
    CallableCodeBlock ccb = program_representation.resolve_function_call(fcall, scope_context.raw_scope_context);
    llvm::Function* llvm_function = callable_codeblocks_llvm_translator.translate_callable_code_block_to_llvm(ccb);
    std::vector<llvm::Value*> llvm_arguments;
    for (size_t arg_index = 0; arg_index < fcall.arguments.size(); arg_index++) {
        const Expression& concrete_arg = fcall.arguments[arg_index];
        TypeManipulationsLLVMTranslator type_manipulations_llvm_translator(program_representation, type_definitions_llvm_translator);
        TranslatedExpression translated_arg = translate_expression_to_llvm(block, concrete_arg);
        auto concrete_expr_type_opt = program_representation.resolve_expression_type(concrete_arg, scope_context.raw_scope_context);
        assert_type_deduction_success_in_backend_layer(concrete_expr_type_opt.has_value());
        TypeSignature concrete_expr_type = concrete_expr_type_opt.value();
        TranslatedExpression casted_arg = type_manipulations_llvm_translator.cast_translated_expression_to_another_type_in_llvm(
            block,
            translated_arg,
            concrete_expr_type,
            ccb.get_arg_types()[arg_index]
        );
        llvm_arguments.push_back(casted_arg.value);
    }
    llvm::IRBuilder<> builder(block);
    return builder.CreateCall(llvm_function, llvm_arguments);
}

llvm::BasicBlock* ExpressionsAndStatementsLLVMTranslator::translate_void_function_call_to_llvm(
    llvm::BasicBlock* block,
    const FunctionCall& function_call
) {
    std::ignore = translate_expr_function_call_to_llvm(block, function_call);
    return block;
}