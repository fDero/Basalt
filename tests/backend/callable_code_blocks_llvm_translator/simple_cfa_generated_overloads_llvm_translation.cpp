
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "core/program_representation.hpp"
#include "backend/callable_codeblocks_llvm_translator.hpp"
#include "../../tests_utilities/function_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

#include <llvm/Support/raw_ostream.h>

#include <iostream>

static FunctionDefinition f_Int_function = FunctionDefinitionFactory::make_void_function_definition(
    "f1",
    "main.basalt",
    FunctionDefinitionFactory::no_generics,
    {
        FunctionDefinition::Argument {
            .arg_name = "x",
            .arg_type = TypeSignatureFactory::Int
        }
    }
);

static FunctionDefinition f_Float_function = FunctionDefinitionFactory::make_void_function_definition(
    "f2",
    "main.basalt",
    FunctionDefinitionFactory::no_generics,
    {
        FunctionDefinition::Argument {
            .arg_name = "x",
            .arg_type = TypeSignatureFactory::Float
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
        .func_defs = { f_Int_function, f_Float_function }
    }
});

static CommonFeatureAdoptionPlanDescriptor f_function_descriptor = CommonFeatureAdoptionPlanDescriptor {
    .function_name = "f",
    .arg_types = { TypeSignatureFactory::IntOrFloat },
    .return_type = std::nullopt,
    .filename = "main.basalt",
    .debug_info = DebugInformationsAwareEntity("main.basalt", 1, 1, 1),
    .plan = RecursiveAdoptionPlan {
        .argument_index = 0,
        .alternatives = { 
            TypeSignatureFactory::Int, 
            TypeSignatureFactory::Float 
        },
        .nested_plans = { 
            std::make_shared<FunctionDefinition>(f_Int_function), 
            std::make_shared<FunctionDefinition>(f_Float_function) 
        }
    }
};

TEST(Backend, CFAForOneArgumentWithTwoAlternativesTranslatedSuccessfully) {
    ProgramRepresentation program_representation(project_with_a_couple_of_simple_functions);
    llvm::LLVMContext context;
    llvm::Module llvm_module("test", context);
    TypeDefinitionsLLVMTranslator type_translator(program_representation, context, llvm_module);
    CallableCodeBlocksLLVMTranslator func_translator(program_representation, type_translator, context, llvm_module);
    CallableCodeBlock f_function_block(f_function_descriptor, program_representation);
    llvm::Function* translated = func_translator.translate_callable_code_block_into_llvm(f_function_block);
    std::string llvm_func_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_func_translation_str);
    translated->print(llvm_ostream);
}