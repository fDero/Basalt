
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "core/program_representation.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

#include <llvm/Support/raw_ostream.h>

static FunctionDefinition f_function = FunctionDefinitionFactory::make_void_function_definition(
    "f",
    "main.basalt",
    FunctionDefinitionFactory::no_generics,
    FunctionDefinitionFactory::no_args
);

static FunctionDefinition g_function = FunctionDefinitionFactory::make_void_function_definition(
    "g",
    "main.basalt",
    FunctionDefinitionFactory::no_generics,
    {
        FunctionDefinition::Argument {
            .arg_name = "x",
            .arg_type = TypeSignatureFactory::Int
        }
    }
);

static FunctionDefinition h_function = FunctionDefinitionFactory::make_void_function_definition(
    "h",
    "main.basalt",
    FunctionDefinitionFactory::no_generics,
    {
        FunctionDefinition::Argument {
            .arg_name = "x",
            .arg_type = TypeSignatureFactory::Int
        },
        FunctionDefinition::Argument {
            .arg_name = "y",
            .arg_type = TypeSignatureFactory::Char
        }
    }
);

static ProjectFileStructure project_with_a_couple_of_simple_functions({
        FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { },
        .func_defs = { f_function }
    }
});

TEST(Backend, FunctionNoGenericsNoBodyNoArgsTranslatedSuccessfully) {
    ProgramRepresentation program_representation(project_with_a_couple_of_simple_functions);
    llvm::LLVMContext context;
    llvm::Module llvm_module("test", context);
    TypeDefinitionsLLVMTranslator type_translator(program_representation, context, llvm_module);
    CallableCodeBlocksLLVMTranslator func_translator(program_representation, type_translator, context, llvm_module);
    FunctionDefinition::Ref f_function_ref = std::make_shared<FunctionDefinition>(f_function);
    CallableCodeBlock f_function_as_callable_code_block(f_function_ref, program_representation);
    llvm::Function* translated = func_translator.translate_callable_code_block_into_llvm(f_function_as_callable_code_block);
    std::string llvm_func_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_func_translation_str);
    translated->print(llvm_ostream);
    EXPECT_EQ(llvm_func_translation_str, "define void @\"f@main.basalt:1:2\"() {\nentry:\n}\n");
}

TEST(Backend, FunctionNoGenericsNoBodyOneIntArgTranslatedSuccessfully) {
    ProgramRepresentation program_representation(project_with_a_couple_of_simple_functions);
    llvm::LLVMContext context;
    llvm::Module llvm_module("test", context);
    TypeDefinitionsLLVMTranslator type_translator(program_representation, context, llvm_module);
    CallableCodeBlocksLLVMTranslator func_translator(program_representation, type_translator, context, llvm_module);
    FunctionDefinition::Ref g_function_ref = std::make_shared<FunctionDefinition>(g_function);
    CallableCodeBlock g_function_as_callable_code_block(g_function_ref, program_representation);
    llvm::Function* translated = func_translator.translate_callable_code_block_into_llvm(g_function_as_callable_code_block);
    std::string llvm_func_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_func_translation_str);
    translated->print(llvm_ostream);
    EXPECT_EQ(llvm_func_translation_str, 
        "define void @\"g@main.basalt:1:2\"(i64 %0) {"   "\n"
        "entry:"                                         "\n"
        "  %1 = alloca i64, align 8"                     "\n"
        "  store i64 %0, i64* %1, align 4"               "\n"
        "}"                                              "\n"
    );
}

TEST(Backend, FunctionNoGenericsNoBodyOneIntArgAndOneCharTranslatedSuccessfully) {
    ProgramRepresentation program_representation(project_with_a_couple_of_simple_functions);
    llvm::LLVMContext context;
    llvm::Module llvm_module("test", context);
    TypeDefinitionsLLVMTranslator type_translator(program_representation, context, llvm_module);
    CallableCodeBlocksLLVMTranslator func_translator(program_representation, type_translator, context, llvm_module);
    FunctionDefinition::Ref h_function_ref = std::make_shared<FunctionDefinition>(h_function);
    CallableCodeBlock h_function_as_callable_code_block(h_function_ref, program_representation);
    llvm::Function* translated = func_translator.translate_callable_code_block_into_llvm(h_function_as_callable_code_block);
    std::string llvm_func_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_func_translation_str);
    translated->print(llvm_ostream);
    EXPECT_EQ(llvm_func_translation_str, 
        "define void @\"h@main.basalt:1:2\"(i64 %0, i8 %1) {"    "\n"
        "entry:"                                                 "\n"
        "  %2 = alloca i64, align 8"                             "\n"
        "  store i64 %0, i64* %2, align 4"                       "\n"
        "  %3 = alloca i8, align 1"                              "\n"
        "  store i8 %1, i8* %3, align 1"                         "\n"
        "}"                                                      "\n"
    );
}