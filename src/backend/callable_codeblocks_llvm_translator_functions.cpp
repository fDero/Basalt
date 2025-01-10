//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/callable_codeblocks_llvm_translator.hpp"

llvm::Function* CallableCodeBlocksLLVMTranslator::translate_pow_builtin_operator_as_llvm_function(
    const BinaryOperator& binary_operator
) {
    throw std::runtime_error("not implemented yet");
}

llvm::Function* CallableCodeBlocksLLVMTranslator::translate_is_builtin_operator_as_llvm_function(
    const TypeOperator& type_operator
) {
    throw std::runtime_error("not implemented yet");
}

llvm::Function* CallableCodeBlocksLLVMTranslator::translate_as_builtin_operator_as_llvm_function(
    const TypeOperator& type_operator
) {
    throw std::runtime_error("not implemented yet");
}

llvm::Function* CallableCodeBlocksLLVMTranslator::translate_function_definition_into_llvm(
    const FunctionDefinition::Ref& function_definition, 
    llvm::Function* llvm_function
) {
    ScopeContext raw_scope_context(*function_definition);
    auto local_variables = std::make_shared<std::map<std::string, llvm::AllocaInst*>>();
    llvm::IRBuilder<> llvm_builder(llvm_context);
    llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(llvm_context, "entry", llvm_function);
    llvm_builder.SetInsertPoint(entry_block);
    for (size_t arg_index = 0; arg_index < function_definition->arguments.size(); arg_index++) {
        const TypeSignature& arg_type = function_definition->arguments[arg_index].arg_type;
        const std::string& arg_name = function_definition->arguments[arg_index].arg_name;
        llvm::Type* llvm_arg_type = type_definitions_llvm_translator
            .translate_typesignature_to_llvm_type(arg_type);
        llvm::AllocaInst* alloca_inst = llvm_builder.CreateAlloca(llvm_arg_type, nullptr);
        llvm_builder.CreateStore(llvm_function->arg_begin() + arg_index, alloca_inst);
        std::string arg_id = raw_scope_context.resolve_object_unique_id(arg_name);
        local_variables->insert({arg_id, alloca_inst});
    }
    TranslationAwareScopeContext scope_context(raw_scope_context, local_variables);
    ExpressionsAndStatementsLLVMTranslator body_translator = 
        get_function_body_translator(scope_context, llvm_function, llvm_builder);
    body_translator.translate_whole_codeblock_into_llvm(function_definition->code);
    return llvm_function;
}