
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "core/program_representation.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

#include <llvm/Support/raw_ostream.h>

static UnionDefinition number_union_definition = 
    UnionDefinitionFactory::make_union_definition(
        "Number", "main.basalt", { }, {
            TypeSignatureFactory::Int,
            TypeSignatureFactory::Float
        }
);

static ProjectFileStructure empty_project({
        FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { number_union_definition },
        .func_defs = { }
    }
});

TEST(Backend, Int_Translated_Correctly) {
    ProgramRepresentation program_representation(empty_project);
    llvm::LLVMContext context;
    llvm::Module llvm_module("IntTranslatedCorrectly", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    llvm::Type* llvm_int_translation = type_definitions_llvm_translator
        .translate_typesignature_to_llvm_type(TypeSignatureFactory::Int);
    std::string llvm_int_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_int_translation_str);
    llvm_int_translation->print(llvm_ostream);
    EXPECT_EQ(llvm_int_translation_str, "i64");
}

TEST(Backend, Float_Translated_Correctly) {
    ProgramRepresentation program_representation(empty_project);
    llvm::LLVMContext context;
    llvm::Module llvm_module("FloatTranslatedCorrectly", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    llvm::Type* llvm_float_translation = type_definitions_llvm_translator
        .translate_typesignature_to_llvm_type(TypeSignatureFactory::Float);
    std::string llvm_float_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_float_translation_str);
    llvm_float_translation->print(llvm_ostream);
    EXPECT_EQ(llvm_float_translation_str, "double");
}

TEST(Backend, Number_Union_Translated_Correctly_From_Definition) {
    ProgramRepresentation program_representation(empty_project);
    llvm::LLVMContext context;
    llvm::Module llvm_module("NumberUnionTranslatedCorrectlyFromDefinition", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    llvm::Type* llvm_number_union_translation = type_definitions_llvm_translator
        .translate_named_union_to_llvm_type(number_union_definition);
    std::string llvm_number_union_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_number_union_translation_str);
    llvm_number_union_translation->print(llvm_ostream);
    EXPECT_EQ(llvm_number_union_translation_str, R"(%"testpackage::Number" = type { i8*, [8 x i8] })");
}

TEST(Backend, IntOrFloat_Translated_Twice_Correctly) {
    ProgramRepresentation program_representation(empty_project);
    llvm::LLVMContext context;
    llvm::Module llvm_module("NumberUnionTranslatedCorrectlyFromDefinition", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    llvm::Type* llvm_number_union_translation1 = type_definitions_llvm_translator
        .translate_typesignature_to_llvm_type(TypeSignatureFactory::IntOrFloat);
    llvm::Type* llvm_number_union_translation2 = type_definitions_llvm_translator
        .translate_typesignature_to_llvm_type(TypeSignatureFactory::IntOrFloat);
    std::string llvm_number_union_translation1_str;
    std::string llvm_number_union_translation2_str;
    llvm::raw_string_ostream llvm_ostream1(llvm_number_union_translation1_str);
    llvm::raw_string_ostream llvm_ostream2(llvm_number_union_translation2_str);
    llvm_number_union_translation1->print(llvm_ostream1);
    llvm_number_union_translation2->print(llvm_ostream2);
    EXPECT_EQ(llvm_number_union_translation1_str, llvm_number_union_translation2_str);
    EXPECT_EQ(llvm_number_union_translation1_str, R"(%"Int | Float" = type { i8*, [8 x i8] })");
    EXPECT_EQ(llvm_number_union_translation2_str, R"(%"Int | Float" = type { i8*, [8 x i8] })");
}

TEST(Backend, IntOrFloat_Translated_Correctly_From_TypeSignature) {
    ProgramRepresentation program_representation(empty_project);
    llvm::LLVMContext context;
    llvm::Module llvm_module("IntOrFloatTranslatedCorrectlyFromDefinition", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    llvm::Type* llvm_int_or_float_union_translation = type_definitions_llvm_translator
        .translate_typesignature_to_llvm_type(TypeSignatureFactory::IntOrFloat);
    std::string llvm_int_or_float_union_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_int_or_float_union_translation_str);
    llvm_int_or_float_union_translation->print(llvm_ostream);
    EXPECT_EQ(llvm_int_or_float_union_translation_str, R"(%"Int | Float" = type { i8*, [8 x i8] })");
}