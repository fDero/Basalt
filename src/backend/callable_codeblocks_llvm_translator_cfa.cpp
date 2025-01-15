//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "errors/internal_errors.hpp"

llvm::Function* CallableCodeBlocksLLVMTranslator::translate_cfa_descriptor_into_llvm(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor, 
    llvm::Function* llvm_function
) {
    llvm::IRBuilder<> llvm_builder(llvm_context);
    llvm::BasicBlock* entry_block = llvm::BasicBlock::Create(llvm_context, "entry", llvm_function);
    llvm_builder.SetInsertPoint(entry_block);
    const std::string cfa_path_prefix = cfa_plan_descriptor.function_name + "_";
    translate_cfa_plan_into_llvm(
        cfa_plan_descriptor, 
        cfa_plan_descriptor.plan, 
        cfa_path_prefix,
        llvm_function, 
        llvm_builder
    );
    return llvm_function;
}

void CallableCodeBlocksLLVMTranslator::translate_cfa_plan_into_llvm(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
    const CommonFeatureAdoptionPlan& cfa_plan, 
    const std::string& cfa_path_prefix,
    llvm::Function* llvm_function,
    llvm::IRBuilder<>& llvm_builder
) {
    if (cfa_plan.is_direct_adoption()) {
        const FunctionDefinition::Ref& concrete_func = cfa_plan.get_direct_adoption();
        handle_direct_cfa_adoption(cfa_plan_descriptor, concrete_func, llvm_function, llvm_builder);
        return;
    }
    const RecursiveAdoptionPlan& recursive_plan = cfa_plan.get_recursive_adoption();
    std::vector<llvm::BasicBlock*> cond_blocks = create_necessary_cfa_cond_blocks(recursive_plan, llvm_function, cfa_path_prefix);
    std::vector<llvm::BasicBlock*> run_blocks = create_necessary_cfa_run_blocks(recursive_plan, llvm_function, cfa_path_prefix);
    cond_blocks.push_back(run_blocks.back());
    populate_cfa_cond_blocks(
        cfa_plan_descriptor, recursive_plan, cond_blocks, 
        run_blocks, llvm_function, llvm_builder
    );
    populate_cfa_run_blocks(
        cfa_plan_descriptor, recursive_plan, cfa_path_prefix,
        run_blocks, llvm_function, llvm_builder
    );
}

std::vector<llvm::BasicBlock*> CallableCodeBlocksLLVMTranslator::create_necessary_cfa_cond_blocks(
    const RecursiveAdoptionPlan& recursive_plan,
    llvm::Function* llvm_function,
    const std::string& cfa_path_prefix
) {
    std::vector<llvm::BasicBlock*> cond_blocks;
    for (size_t alternative_counter = 0; alternative_counter < recursive_plan.alternatives.size() - 1; ++alternative_counter) {
        const std::string cond_block_name = cfa_path_prefix + "cond_" + std::to_string(alternative_counter) + "_";
        auto alternative_specific_cond_block = llvm::BasicBlock::Create(llvm_context, cond_block_name, llvm_function);   
        cond_blocks.push_back(alternative_specific_cond_block);
    }
    return cond_blocks;
}

std::vector<llvm::BasicBlock*> CallableCodeBlocksLLVMTranslator::create_necessary_cfa_run_blocks(
    const RecursiveAdoptionPlan& recursive_plan,
    llvm::Function* llvm_function,
    const std::string& cfa_path_prefix
) {
    std::vector<llvm::BasicBlock*> cond_blocks;
    for (size_t alternative_counter = 0; alternative_counter < recursive_plan.alternatives.size(); ++alternative_counter) {
        const std::string cond_block_name = cfa_path_prefix + "run_" + std::to_string(alternative_counter) + "_";
        auto alternative_specific_cond_block = llvm::BasicBlock::Create(llvm_context, cond_block_name, llvm_function);   
        cond_blocks.push_back(alternative_specific_cond_block);
    }
    return cond_blocks;
}

void CallableCodeBlocksLLVMTranslator::populate_cfa_cond_blocks(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
    const RecursiveAdoptionPlan& recursive_plan,
    const std::vector<llvm::BasicBlock*>& cond_blocks,
    const std::vector<llvm::BasicBlock*>& run_blocks,
    llvm::Function* llvm_function,
    llvm::IRBuilder<>& llvm_builder
) {
    throw std::runtime_error("Not implemented yet");
    //for (size_t alternative_counter = 0; alternative_counter < recursive_plan.alternatives.size() - 1; ++alternative_counter) {
    //    auto cond_block = cond_blocks[alternative_counter];
    //    llvm_builder.SetInsertPoint(cond_block);
    //    std::string argument_as_basalt_identifier = "arg_" + std::to_string(recursive_plan.argument_index);
    //    TypeOperator is_operator(
    //        cfa_plan_descriptor.debug_info, 
    //        "is", 
    //        Identifier(cfa_plan_descriptor.debug_info, argument_as_basalt_identifier),
    //        recursive_plan.alternatives[alternative_counter]
    //    );
    //    llvm::Function* is_builtin_function = translate_is_builtin_operator_as_llvm_function(is_operator);
    //    llvm::Value* llvm_argument = llvm_function->arg_begin() + recursive_plan.argument_index;
    //    llvm::Value* is_result = llvm_builder.CreateCall(is_builtin_function, {llvm_argument});
    //    auto run_block = run_blocks[alternative_counter];
    //    auto next_cond_block = cond_blocks[alternative_counter + 1];
    //    llvm_builder.CreateCondBr(is_result, run_block, next_cond_block);
    //}
}

void CallableCodeBlocksLLVMTranslator::populate_cfa_run_blocks(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
    const RecursiveAdoptionPlan& recursive_plan,
    const std::string& cfa_path_prefix,
    const std::vector<llvm::BasicBlock*>& run_blocks,
    llvm::Function* llvm_function,
    llvm::IRBuilder<>& llvm_builder
) {
    for (size_t alternative_counter = 0; alternative_counter < recursive_plan.alternatives.size(); ++alternative_counter) {
        auto run_block = run_blocks[alternative_counter];
        llvm_builder.SetInsertPoint(run_block);
        std::string new_path_prefix = cfa_path_prefix + "arg_" + std::to_string(recursive_plan.argument_index) + "_";
        new_path_prefix = cfa_path_prefix + "choice_" + std::to_string(alternative_counter) + "_";
        const CommonFeatureAdoptionPlan& plan = recursive_plan.nested_plans[alternative_counter];
        translate_cfa_plan_into_llvm(cfa_plan_descriptor, plan, cfa_path_prefix, llvm_function, llvm_builder);
    }
}

void CallableCodeBlocksLLVMTranslator::handle_direct_cfa_adoption(
    const CommonFeatureAdoptionPlanDescriptor& cfa_plan_descriptor,
    const FunctionDefinition::Ref& selected_concrete_function,
    llvm::Function* llvm_function,
    llvm::IRBuilder<>& llvm_builder
) {
    auto ccblock = CallableCodeBlock(selected_concrete_function, program_representation);
    llvm::Function* concrete_function = translate_callable_code_block_into_llvm(ccblock);
    std::vector<llvm::Value*> arguments;
    for (size_t arg_index = 0; arg_index < selected_concrete_function->arguments.size(); arg_index++) {
        const TypeSignature& actual_arg_type = cfa_plan_descriptor.arg_types[arg_index];
        const TypeSignature& expected_arg_type = selected_concrete_function->arguments[arg_index].arg_type;
        bool is_assignable = program_representation.validate_assignment(expected_arg_type, actual_arg_type);
        bool is_union = program_representation.is_union(expected_arg_type);
        if (is_assignable || is_union) {
            arguments.push_back(llvm_function->arg_begin() + arg_index);
            continue;
        }
        bool is_union_source = program_representation.is_union(actual_arg_type);
        bool is_union_target = program_representation.is_union(expected_arg_type);
        assert_is_assignment_of_non_union_to_union(is_union_source, is_union_target);
        llvm::Value* union_payload = llvm_builder.CreateGEP(llvm_function->arg_begin() + arg_index, {0, 1});
        arguments.push_back(union_payload);
    }
    llvm::Value* return_value = llvm_builder.CreateCall(concrete_function, arguments);
    if (cfa_plan_descriptor.return_type.has_value()) {
        llvm_builder.CreateRet(return_value);
        return;
    }
    llvm_builder.CreateRetVoid();
}