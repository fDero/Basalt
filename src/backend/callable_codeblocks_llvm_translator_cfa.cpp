//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "backend/type_operators_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

llvm::Function* CallableCodeBlocksLLVMTranslator::translate_cfa_descriptor_into_llvm(
    const CommonFeatureAdoptionPlanDescriptor& descriptor, 
    llvm::Function* llvm_function
) {
    llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(llvm_context, "entry", llvm_function);
    translate_cfa_plan_into_llvm(descriptor, descriptor.plan, llvm_function, entry_block);
    return llvm_function;
}

void CallableCodeBlocksLLVMTranslator::translate_cfa_plan_into_llvm(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor, 
    const CommonFeatureAdoptionPlan& cfa_plan,
    llvm::Function* llvm_function,
    llvm::BasicBlock* block
) {
    if (cfa_plan.is_direct_adoption()) {
        FunctionDefinition::Ref selected_concrete_function = cfa_plan.get_direct_adoption();
        translate_cfa_direct_adoption_into_llvm(cfa_plan_descriptor, selected_concrete_function, llvm_function, block);
        return;
    }
    assert(cfa_plan.is_recursive_adoption());
    RecursiveAdoptionPlan recursive_plan = cfa_plan.get_recursive_adoption();
    translate_cfa_recursive_adoption_into_llvm(cfa_plan_descriptor, recursive_plan, llvm_function, block);
}

void CallableCodeBlocksLLVMTranslator::translate_cfa_recursive_adoption_into_llvm(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor, 
    const RecursiveAdoptionPlan& recursive_plan,
    llvm::Function* llvm_function,
    llvm::BasicBlock* block
) {
    std::vector<llvm::BasicBlock*> alternative_blocks { block };
    for (size_t alternative_index = 1; alternative_index < recursive_plan.alternatives.size(); alternative_index++) {
        llvm::BasicBlock* prev_block = alternative_blocks.back();
        llvm::BasicBlock* alternative_block = llvm::BasicBlock::Create(llvm_context, "", llvm_function);
        alternative_block->moveAfter(prev_block);
        alternative_blocks.push_back(alternative_block);
    }
    for (size_t alternative_index = 0; alternative_index < recursive_plan.alternatives.size() - 1; alternative_index++) {
        llvm::BasicBlock* alternative_block = alternative_blocks[alternative_index];
        llvm::IRBuilder<> alternative_block_builder(alternative_block);
        llvm::Value* llvm_argument = llvm_function->getArg(recursive_plan.argument_index);
        TypeOperatorsLLVMTranslator type_operators_llvm_translator(program_representation, type_definitions_llvm_translator);
        TranslatedExpression is_operator = type_operators_llvm_translator.translate_is_operator_to_llvm_value(
            alternative_block, 
            llvm_argument, 
            recursive_plan.alternatives[alternative_index]
        );
        llvm::BasicBlock* success_block = llvm::BasicBlock::Create(llvm_context, "", llvm_function);
        success_block->moveAfter(alternative_block);
        alternative_block_builder.CreateCondBr(is_operator.value, success_block, alternative_blocks[alternative_index + 1]);
        const CommonFeatureAdoptionPlan& successful_plan = recursive_plan.nested_plans[alternative_index];
        translate_cfa_plan_into_llvm(cfa_plan_descriptor, successful_plan, llvm_function, success_block);
    }
    llvm::BasicBlock* last_alternative_block = alternative_blocks.back();
    const CommonFeatureAdoptionPlan& last_alternative_plan = recursive_plan.nested_plans.back();
    translate_cfa_plan_into_llvm(cfa_plan_descriptor, last_alternative_plan, llvm_function, last_alternative_block);
}

void CallableCodeBlocksLLVMTranslator::translate_cfa_direct_adoption_into_llvm(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
    const FunctionDefinition::Ref& selected_concrete_function,
    llvm::Function* llvm_function,
    llvm::BasicBlock* block
) {
    llvm::IRBuilder<> llvm_builder(block);
    auto ccblock = CallableCodeBlock(selected_concrete_function, program_representation);
    llvm::Function* concrete_function = translate_callable_code_block_into_llvm(ccblock);
    std::vector<llvm::Value*> arguments;
    for (size_t arg_index = 0; arg_index < selected_concrete_function->arguments.size(); arg_index++) {
        const TypeSignature& expected_arg_type = selected_concrete_function->arguments[arg_index].arg_type;
        TypeOperatorsLLVMTranslator type_operators_llvm_translator(program_representation, type_definitions_llvm_translator);
        llvm::Type* expected_arg_llvm_type = type_definitions_llvm_translator.translate_typesignature_to_llvm_type(expected_arg_type);
        TranslatedExpression as_operator = type_operators_llvm_translator.translate_as_operator_to_llvm_value(
            llvm_builder.GetInsertBlock(), 
            llvm_function->getArg(arg_index),
            expected_arg_llvm_type, 
            expected_arg_type
        );
        arguments.push_back(as_operator.value);
    }
    llvm::Value* return_value = llvm_builder.CreateCall(concrete_function, arguments);
    if (cfa_plan_descriptor.return_type.has_value()) {
        llvm_builder.CreateRet(return_value);
        return;
    }
    llvm_builder.CreateRetVoid();
}