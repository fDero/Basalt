//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/callable_codeblocks_llvm_translator.hpp"

CallableCodeBlocksLLVMTranslator::CallableCodeBlocksLLVMTranslator(
    ProgramRepresentation& program_representation, 
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
    llvm::LLVMContext& llvm_context,
    llvm::Module& llvm_module
)
    : program_representation(program_representation) 
    , type_definitions_llvm_translator(type_definitions_llvm_translator)
    , llvm_context(llvm_context)
    , llvm_module(llvm_module)
{ }

llvm::Function* CallableCodeBlocksLLVMTranslator::translate_function_definition_into_llvm(
    const FunctionDefinition::Ref& function_definition, 
    llvm::Function* llvm_function
) {
    ScopeContext raw_scope_context(*function_definition);
    auto local_variables = std::make_shared<std::map<std::string, llvm::AllocaInst*>>();
    llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(llvm_context, "entry", llvm_function);
    llvm::IRBuilder<> llvm_builder(entry_block);
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
        get_function_body_translator(scope_context, llvm_function, entry_block, llvm_builder);
    body_translator.translate_whole_codeblock_into_llvm(entry_block, function_definition->code);
    return llvm_function;
}

llvm::Function* CallableCodeBlocksLLVMTranslator::translate_callable_code_block_into_llvm(
    const CallableCodeBlock& callable_code_block
) {
    const std::string& llvm_func_name = callable_code_block.unique_context_independent_id; 
    auto llvm_function_search_outcome = llvm_functions.find(llvm_func_name);
    if (llvm_function_search_outcome != llvm_functions.end()) {
        return llvm_function_search_outcome->second;
    }
    const std::optional<TypeSignature>& return_type = callable_code_block.get_return_type();
    llvm::Type* llvm_return_type = type_definitions_llvm_translator.translate_return_type_to_llvm_type(return_type);
    const std::vector<TypeSignature>& arg_types = callable_code_block.get_arg_types();
    const bool constexpr is_vararg = false;
    std::vector<llvm::Type*> llvm_arg_types = type_definitions_llvm_translator.translate_all_types_to_llvm_types(arg_types);
    auto function_signature = llvm::FunctionType::get(llvm_return_type, llvm_arg_types, is_vararg);
    auto linkage_policy = llvm::Function::ExternalLinkage;
    llvm::Function* llvm_function = llvm::Function::Create(function_signature, linkage_policy, llvm_func_name, llvm_module);
    llvm_functions.insert({llvm_func_name, llvm_function});
    switch (callable_code_block.callable_codeblock_kind()) {
        case CallableCodeBlock::Kind::function_definition: 
            return translate_function_definition_into_llvm(
                callable_code_block.get<FunctionDefinition::Ref>(), llvm_function
            );
        case CallableCodeBlock::Kind::common_feature_adoption_plan:
            return translate_cfa_descriptor_into_llvm(
                callable_code_block.get<CommonFeatureAdoptionPlanDescriptor>(), llvm_function
            );
    }
    assert_unreachable();
}

ExpressionsAndStatementsLLVMTranslator 
CallableCodeBlocksLLVMTranslator::get_function_body_translator(
    TranslationAwareScopeContext scope_context,
    llvm::Function* llvm_function,
    llvm::BasicBlock* function_entry_block,
    llvm::IRBuilder<>& llvm_builder
) {
    return ExpressionsAndStatementsLLVMTranslator(
        program_representation, 
        type_definitions_llvm_translator,
        *this,
        scope_context,
        llvm_context,
        llvm_function,
        function_entry_block
    );
}