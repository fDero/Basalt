
#include <gtest/gtest.h>
#include "errors/internal_errors.hpp"
#include "core/program_representation.hpp"
#include "backend/type_definitions_llvm_translator.hpp"
#include "../../tests_utilities/union_definition_factory.hpp"
#include "../../tests_utilities/struct_definition_factory.hpp"
#include "../../tests_utilities/typesignature_factory.hpp"
#include "../../tests_utilities/type_alias_factory.hpp"

#include <llvm/Support/raw_ostream.h>

static StructDefinition pair_generic_struct = 
    StructDefinitionFactory::make_struct_definition(
        "Pair", "main.basalt", { "T" }, {
            StructDefinition::Field { "first", TypeSignatureFactory::T },
            StructDefinition::Field { "second", TypeSignatureFactory::T },
        }
);

static ProjectFileStructure project_with_a_couple_of_simple_functions({
        FileRepresentation {
        .file_metadata = {
            .filename = "main.basalt",
            .packagename = "testpackage",
            .imports = { }
        },
        .type_defs = { 
            pair_generic_struct
        },
        .func_defs = { }
    }
});

TEST(Backend, Generic_Pair_Instantiated_With_Int_Translated_Correctly) {
    ProgramRepresentation program_representation(project_with_a_couple_of_simple_functions);
    llvm::LLVMContext context;
    llvm::Module llvm_module("IntTranslatedCorrectly", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    llvm::Type* llvm_pair_translation = type_definitions_llvm_translator
        .translate_typesignature_to_llvm_type(
            CustomType { 
                Token { "Pair", "main.basalt", 1, 1, 4, Token::Type::type },
                { TypeSignatureFactory::Int } 
            }
        );
    std::string llvm_pair_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_pair_translation_str);
    llvm_pair_translation->print(llvm_ostream);
    EXPECT_EQ(llvm_pair_translation_str, R"(%"testpackage::Pair<Int>" = type { i64, i64 })");
}

TEST(Backend, Generic_Pair_Instantiated_With_Char_Translated_Correctly) {
    ProgramRepresentation program_representation(project_with_a_couple_of_simple_functions);
    llvm::LLVMContext context;
    llvm::Module llvm_module("IntTranslatedCorrectly", context);
    TypeDefinitionsLLVMTranslator type_definitions_llvm_translator(
        program_representation,
        context,
        llvm_module
    );
    llvm::Type* llvm_pair_translation = type_definitions_llvm_translator
        .translate_typesignature_to_llvm_type(
            CustomType { 
                Token { "Pair", "main.basalt", 1, 1, 4, Token::Type::type },
                { TypeSignatureFactory::Char } 
            }
        );
    std::string llvm_pair_translation_str;
    llvm::raw_string_ostream llvm_ostream(llvm_pair_translation_str);
    llvm_pair_translation->print(llvm_ostream);
    EXPECT_EQ(llvm_pair_translation_str, R"(%"testpackage::Pair<Char>" = type { i8, i8 })");
}