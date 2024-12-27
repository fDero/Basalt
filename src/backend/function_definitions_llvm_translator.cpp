//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/function_definitions_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

FunctionDefinitionsLLVMTranslator::FunctionDefinitionsLLVMTranslator(
    TypeDefinitionsLLVMTranslator& type_definitions_llvm_translator,
    ProgramRepresentation& program_representation, 
    llvm::LLVMContext& context,
    llvm::Module& llvm_module
)
    : type_definitions_llvm_translator(type_definitions_llvm_translator)
    , program_representation(program_representation) 
    , context(context)
    , llvm_module(llvm_module)
{ }

llvm::Type* FunctionDefinitionsLLVMTranslator::translate_return_type_to_llvm_type(
    const FunctionDefinition::Ref& function_definition
) {
    if (!function_definition->return_type.has_value()) {
        return llvm::Type::getVoidTy(context);
    }
    const TypeSignature& ret = function_definition->return_type.value();
    return type_definitions_llvm_translator.translate_typesignature_to_llvm_type(ret);
}

std::vector<llvm::Type*> FunctionDefinitionsLLVMTranslator::translate_arguments_to_llvm_types(
    const FunctionDefinition::Ref& function_definition
) {
    std::vector<llvm::Type*> llvm_arg_types;
    for (const FunctionDefinition::Argument& arg : function_definition->arguments) {
        llvm::Type* llvm_arg_type = type_definitions_llvm_translator
            .translate_typesignature_to_llvm_type(arg.arg_type);
        llvm_arg_types.push_back(llvm_arg_type);
    }
    return llvm_arg_types;
}

llvm::Function* FunctionDefinitionsLLVMTranslator::translate_function_definition_to_llvm(
    const FunctionDefinition::Ref& function_definition
){
    llvm::Type* llvm_return_type = translate_return_type_to_llvm_type(function_definition);
    std::vector<llvm::Type*> llvm_arg_types = translate_arguments_to_llvm_types(function_definition);
    const bool constexpr is_vararg = false;
    auto function_signature = llvm::FunctionType::get(llvm_return_type, is_vararg);
	auto linkage_policy = llvm::Function::ExternalLinkage;
    const std::string llvm_function_name = function_definition->as_debug_informations_aware_entity().unique_string_id();
    llvm::Function* function = llvm::Function::Create(
        function_signature, 
        linkage_policy, 
        llvm_function_name, 
        llvm_module
    );
    return function;
}