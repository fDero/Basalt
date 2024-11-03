//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// AUTHOR: Francesco De Rosa (https://github.com/fDero)                    //
// LICENSE: MIT (https://github.com/fDero/Basalt/blob/master/LICENSE)      //
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "errors/display_utilities.hpp"
#include "frontend/commandline.hpp"

#include <exception>
#include <iostream>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#define VARARG true
#define NON_VARARG false
#define EXIT_SUCCESS 0

void llvm_demo(){
    std::string module_name = "demo";
    std::string output_file_name = "demo.ll";

    llvm::LLVMContext context;
    llvm::Module module (module_name, context);
    llvm::IRBuilder<> builder(context);

    auto int32_type = builder.getInt32Ty();
    auto byte_pointer_type = builder.getInt8Ty()->getPointerTo();

    llvm::Constant* hello_world_str = llvm::ConstantDataArray::getString(context, "Hello, World!\n", true);
    auto hello_world_var = new llvm::GlobalVariable(module, hello_world_str->getType(), true, llvm::GlobalValue::PrivateLinkage, hello_world_str);
    auto hello_world_ptr = builder.CreateConstGEP2_32(hello_world_var->getValueType(), hello_world_var, 0, 0);

    llvm::Type* printf_types[] = {byte_pointer_type};
    llvm::FunctionType* printf_type = llvm::FunctionType::get(int32_type, printf_types, VARARG);
    module.getOrInsertFunction("printf", printf_type);

    auto main_function_type = llvm::FunctionType::get(int32_type, NON_VARARG);
    auto linkage_policy = llvm::Function::ExternalLinkage;
    auto main_function = llvm::Function::Create(main_function_type, linkage_policy, "main", module);

    auto main_function_code_block = llvm::BasicBlock::Create(context, "entry", main_function);
    builder.SetInsertPoint(main_function_code_block);

    builder.CreateCall(module.getFunction("printf"), {hello_world_ptr});

    auto result = builder.getInt32(EXIT_SUCCESS);
    auto i32Result = builder.CreateIntCast(result, builder.getInt32Ty(), true);
    builder.CreateRet(i32Result);

    std::cout << "Emitting output..." << std::endl;
    std::error_code error_code;
    llvm::raw_fd_ostream output_file (output_file_name, error_code);
    module.print(output_file, nullptr);
    module.print(llvm::outs(), nullptr);
}

int main(int argc, char** argv) {
    llvm_demo();
    try {
        CommandLineController controller(argc, argv);
        controller.dispatch_based_on_user_input();
        exit(EXIT_SUCCESS);
    }
    catch(...) {
        std::exception_ptr error = std::current_exception();
        display_pretty_error_message(error);
        exit(EXIT_FAILURE);
    }
}
