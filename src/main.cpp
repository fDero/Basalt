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
    std::string mymodule_name = "demo";
    std::string output_file_name = "demo.ll";

    llvm::LLVMContext context;
    llvm::Module mymodule (mymodule_name, context);
    llvm::IRBuilder builder(context);

    auto int32_type = builder.getInt32Ty();
    auto byte_pointer_type = builder.getInt8Ty()->getPointerTo();

    llvm::Constant* hello_world_str = llvm::ConstantDataArray::getString(context, "Hello, World!\n", true);
    auto hello_world_var = new llvm::GlobalVariable(mymodule, hello_world_str->getType(), true, llvm::GlobalValue::PrivateLinkage, hello_world_str);
    auto hello_world_ptr = builder.CreateConstGEP2_32(hello_world_var->getValueType(), hello_world_var, 0, 0);

    llvm::Type* printf_types[] = {byte_pointer_type};
    llvm::FunctionType* printf_type = llvm::FunctionType::get(int32_type, printf_types, VARARG);
    mymodule.getOrInsertFunction("printf", printf_type);

    auto main_function_type = llvm::FunctionType::get(int32_type, NON_VARARG);
    auto linkage_policy = llvm::Function::ExternalLinkage;
    auto main_function = llvm::Function::Create(main_function_type, linkage_policy, "main", mymodule);

    auto main_function_code_block = llvm::BasicBlock::Create(context, "entry", main_function);
    builder.SetInsertPoint(main_function_code_block);

    builder.CreateCall(mymodule.getFunction("printf"), {hello_world_ptr});

    auto result = builder.getInt32(EXIT_SUCCESS);
    auto i32Result = builder.CreateIntCast(result, builder.getInt32Ty(), true);
    builder.CreateRet(i32Result);

    std::cout << "Emitting output..." << std::endl;
    std::error_code error_code;
    llvm::raw_fd_ostream output_file (output_file_name, error_code);
    mymodule.print(output_file, nullptr);
    mymodule.print(llvm::outs(), nullptr);
}

void llvm_demo2(){
    std::string mymodule_name = "demo";
    std::string output_file_name = "demo.ll";

    llvm::LLVMContext context;
    llvm::Module mymodule (mymodule_name, context);
    llvm::IRBuilder builder(context);

    auto int32_type = builder.getInt32Ty();
    auto byte_pointer_type = builder.getInt8Ty()->getPointerTo();
    
    llvm::Type* member_types[] = { builder.getInt64Ty(), builder.getInt64Ty() };
    llvm::StructType* myStructType = llvm::StructType::create(context, member_types, "MyStruct");
    mymodule.getOrInsertGlobal("myStruct", myStructType);


    llvm::Type* printf_types[] = { byte_pointer_type };
    llvm::FunctionType* printf_type = llvm::FunctionType::get(int32_type, printf_types, VARARG);
    mymodule.getOrInsertFunction("printf", printf_type);

    auto main_function_type = llvm::FunctionType::get(int32_type, NON_VARARG);
    auto linkage_policy = llvm::Function::ExternalLinkage;
    auto main_function = llvm::Function::Create(main_function_type, linkage_policy, "main", mymodule);

    auto main_function_code_block = llvm::BasicBlock::Create(context, "entry", main_function);
    builder.SetInsertPoint(main_function_code_block);

    //builder.CreateCall(mymodule.getFunction("printf"), {hello_world_ptr});

    auto result = builder.getInt32(EXIT_SUCCESS);
    auto i32Result = builder.CreateIntCast(result, builder.getInt32Ty(), true);
    builder.CreateRet(i32Result);

    std::cout << "Emitting output..." << std::endl;
    std::error_code error_code;
    llvm::raw_fd_ostream output_file (output_file_name, error_code);
    mymodule.print(output_file, nullptr);
    mymodule.print(llvm::outs(), nullptr);
}


void llvm_demo3(){
    std::string mymodule_name = "demo";
    std::string output_file_name = "demo.ll";

    llvm::LLVMContext context;
    llvm::Module mymodule (mymodule_name, context);
    llvm::IRBuilder builder(context);

    auto int32_type = builder.getInt32Ty();
    auto byte_pointer_type = builder.getInt8Ty()->getPointerTo();
    
    llvm::Type* member_types[] = { builder.getInt64Ty(), builder.getInt64Ty() };
    llvm::StructType* myStructType = llvm::StructType::create(context, member_types, "MyStruct");


    llvm::Type* member_types2[] = { myStructType };
    llvm::StructType* myStructType2 = llvm::StructType::create(context, member_types2, "MyStruct2");

    llvm::Type* printf_types[] = { byte_pointer_type };
    llvm::FunctionType* printf_type = llvm::FunctionType::get(int32_type, printf_types, VARARG);
    mymodule.getOrInsertFunction("printf", printf_type);

    llvm::Constant* int_template_string = llvm::ConstantDataArray::getString(context, "%lld\n", true);
    auto int_template_string_var = new llvm::GlobalVariable(mymodule, int_template_string->getType(), true, llvm::GlobalValue::PrivateLinkage, int_template_string);
    auto int_template_string_ptr = builder.CreateConstGEP2_32(int_template_string_var->getValueType(), int_template_string_var, 0, 0);

    auto main_function_type = llvm::FunctionType::get(int32_type, NON_VARARG);
    auto linkage_policy = llvm::Function::ExternalLinkage;
    auto main_function = llvm::Function::Create(main_function_type, linkage_policy, "main", mymodule);

    auto main_function_code_block = llvm::BasicBlock::Create(context, "entry", main_function);
    builder.SetInsertPoint(main_function_code_block);

    llvm::AllocaInst* structAlloc = builder.CreateAlloca(myStructType, nullptr, "myStruct");
    
    llvm::AllocaInst* structAlloc2 = builder.CreateAlloca(myStructType2, nullptr, "myStruct2");
    

    llvm::Value* member0Ptr = builder.CreateGEP(myStructType, structAlloc, { builder.getInt32(0), builder.getInt32(0) }, "member0");
    llvm::Value* member1Ptr = builder.CreateGEP(myStructType, structAlloc, { builder.getInt32(0), builder.getInt32(1) }, "member1");
    builder.CreateStore(builder.getInt64(42), member0Ptr);
    builder.CreateStore(builder.getInt64(84), member1Ptr);
    llvm::Value* loadedValue = builder.CreateLoad(member0Ptr, "loadedValue");
    builder.CreateCall(mymodule.getFunction("printf"), { int_template_string_ptr, loadedValue });

    auto result = builder.getInt32(EXIT_SUCCESS);
    auto i32Result = builder.CreateIntCast(result, builder.getInt32Ty(), true);
    builder.CreateRet(i32Result);

    std::cout << "Emitting output..." << std::endl;
    std::error_code error_code;
    llvm::raw_fd_ostream output_file (output_file_name, error_code);
    mymodule.print(output_file, nullptr);
    mymodule.print(llvm::outs(), nullptr);
}


void llvm_demo4(){
    std::string mymodule_name = "demo";
    std::string output_file_name = "demo.ll";

    llvm::LLVMContext context;
    llvm::Module mymodule (mymodule_name, context);
    llvm::IRBuilder builder(context);

    auto int32_type = builder.getInt32Ty();
    auto byte_pointer_type = builder.getInt8Ty()->getPointerTo();

    
    llvm::StructType* Rec1 = llvm::StructType::create(context, "Rec1::A");
    llvm::StructType* Rec2 = llvm::StructType::create(context, "Rec2");
    llvm::StructType* myStructType = llvm::StructType::create(context, "MyStruct");

    
    Rec1->setBody({Rec2->getPointerTo()});
    Rec2->setBody({Rec1});
    myStructType->setBody({ builder.getInt64Ty(), builder.getInt64Ty() });

    llvm::Type* printf_types[] = { byte_pointer_type };
    llvm::FunctionType* printf_type = llvm::FunctionType::get(int32_type, printf_types, VARARG);
    mymodule.getOrInsertFunction("printf", printf_type);

    llvm::Constant* int_template_string = llvm::ConstantDataArray::getString(context, "%lld\n", true);
    auto int_template_string_var = new llvm::GlobalVariable(mymodule, int_template_string->getType(), true, llvm::GlobalValue::PrivateLinkage, int_template_string);
    auto int_template_string_ptr = builder.CreateConstGEP2_32(int_template_string_var->getValueType(), int_template_string_var, 0, 0);

    auto main_function_type = llvm::FunctionType::get(int32_type, NON_VARARG);
    auto linkage_policy = llvm::Function::ExternalLinkage;
    auto main_function = llvm::Function::Create(main_function_type, linkage_policy, "main", mymodule);

    auto main_function_code_block = llvm::BasicBlock::Create(context, "entry", main_function);
    builder.SetInsertPoint(main_function_code_block);

    llvm::AllocaInst* structAlloc = builder.CreateAlloca(myStructType, nullptr, "myStruct");
    llvm::AllocaInst* structAlloc2 = builder.CreateAlloca(Rec1, nullptr, "Rec1");

    llvm::Value* member0Ptr = builder.CreateGEP(myStructType, structAlloc, { builder.getInt32(0), builder.getInt32(0) }, "member0");
    llvm::Value* member1Ptr = builder.CreateGEP(myStructType, structAlloc, { builder.getInt32(0), builder.getInt32(1) }, "member1");
    builder.CreateStore(builder.getInt64(42), member0Ptr);
    builder.CreateStore(builder.getInt64(84), member1Ptr);
    llvm::Value* loadedValue = builder.CreateLoad(member0Ptr, "loadedValue");
    builder.CreateCall(mymodule.getFunction("printf"), { int_template_string_ptr, loadedValue });

    auto result = builder.getInt32(EXIT_SUCCESS);
    auto i32Result = builder.CreateIntCast(result, builder.getInt32Ty(), true);
    builder.CreateRet(i32Result);

    std::cout << "Emitting output..." << std::endl;
    std::error_code error_code;
    llvm::raw_fd_ostream output_file (output_file_name, error_code);
    mymodule.print(output_file, nullptr);
    mymodule.print(llvm::outs(), nullptr);
}


int main(int argc, char** argv) {
    llvm_demo4();
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
