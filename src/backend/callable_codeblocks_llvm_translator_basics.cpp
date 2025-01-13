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

ExpressionsAndStatementsLLVMTranslator 
CallableCodeBlocksLLVMTranslator::get_function_body_translator(
    TranslationAwareScopeContext scope_context,
    llvm::Function* llvm_function,
    llvm::IRBuilder<>& llvm_builder
) {
    return ExpressionsAndStatementsLLVMTranslator(
        program_representation, 
        type_definitions_llvm_translator,
        *this,
        scope_context,
        llvm_context,
        llvm_builder,
        llvm_function
    );
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
    auto main_function_type = llvm::FunctionType::get(llvm_return_type, llvm_arg_types, is_vararg);
    auto linkage_policy = llvm::Function::ExternalLinkage;
    llvm::Function* llvm_function = llvm::Function::Create(main_function_type, linkage_policy, llvm_func_name, llvm_module);
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